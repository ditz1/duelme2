#include <game_state.hpp>
#include <chrono>
#include <thread>

int lowest_stage_y_level = 0;
std::array<Rectangle, 4> player_hitboxes;
std::array<Rectangle, 4> player_hurtboxes;
std::array<bool, 4> processed_hit;
std::array<int, 4> player_faces;

void UpdateGameStateWithoutRequest() {
    // if somehow this is triggered, dont break the game
    // but log that it happened and most likely it was desync,
    // so eventually add some kind of resolve strategy but this should still never happen
    std::cout << " this should never be logging (UpdateGameStateWithoutRequest() - server/src/game_state.cpp)" << std::endl;
    for (int i = 0; i < 4; i++){
        switch(PlayerState(game_state.player_states[i])){
            case MOVE_RIGHT: game_state.player_positions[i].x += 2; break;
            case MOVE_LEFT: game_state.player_positions[i].x -= 2; break;
            case MOVE_UP: game_state.player_positions[i].y -= 2; break;
            case MOVE_DOWN: game_state.player_positions[i].y += 2; break;
            case IDLE:
                break;
            default:
                break;
        }
    }
    std::array<uint8_t, 32> response;
    response[0] = msg_update;
    std::array<uint8_t, 28> updated_game_bytes = game_state.ToBytes();
    std::copy(updated_game_bytes.begin(), updated_game_bytes.end(), response.begin() + 1);
    response[29] = msg_end;
    response[30] = msg_from_server;
    response[31] = msg_end;
    BroadcastMessage(response);

}


void UpdateGameState(std::array<uint8_t, 32>& message, ServerStage& stage) {
    if (message[0] != msg_update) return; 

    std::array<uint8_t, 32> response;    
    
    std::array<uint8_t, 28> curr_game_bytes = game_state.ToBytes();

    ParseGameStateRequest(curr_game_bytes, message, game_state, stage);

    std::array<uint8_t, 28> updated_game_bytes = game_state.ToBytes();

    response[0] = msg_update;
    std::copy(updated_game_bytes.begin(), updated_game_bytes.end(), response.begin() + 1);
    response[29] = msg_end;
    response[30] = msg_from_server;
    response[31] = msg_end;
    BroadcastMessage(response);
}

void LoadStageData(std::array<uint8_t, 32>& message){
    if (message[0] != msg_lobby) return;
    if (message[2] != 0) return;
    if (message[4] == msg_end) {
        std::cout << "starting to load stage data " << loading_stage_phase << std::endl;
        return;
    }
    std::cout << "Loading stage data" << std::endl;

}

void BroadcastStageData() {
    std::cout << "broadcasting stage data " << loading_stage_phase << std::endl;

}

void ParseSerialStageData(std::array<uint8_t, 32>& message, ServerStage& stage){
    if (message[0] != msg_stage_data) return;
    if (message[1] == msg_end_stage_data) {
        stage.LoadDataIntoCells();
        for (Rectangle& cell : stage.cells){
            printf("x: %d y: %d width: %d height: %d\n", cell.x, cell.y, cell.width, cell.height);
        }
        printf("scale: %f player width: %d player height: %d\n", stage.scale, stage.player_width, stage.player_height);
        // save max y level (lowest block) for player gravity
        for (Rectangle cell : stage.cells){
            if (cell.y - cell.height > lowest_stage_y_level){
                lowest_stage_y_level = cell.y;
            }
        }

        loading_stage_phase = 2;
        ChangeGameState();
        std::cout << "recieved end stage data, starting game" << std::endl;
        return;
    }

    //std::cout << "parsing serial stage data" << std::endl;
    //for (uint8_t c : message){
    //    printf("%x ", c);
    //}
    for (int i = 2; i < 32; i++){
        stage.data.push_back(message[i]);
    }
    std::cout << std::endl;
    
}

void UpdateLobbyState(std::array<uint8_t, 32>& message) {
    if (message[0] != msg_lobby) return;
    if (message[1] == msg_load_stage_grid && message[2] == 0) {
        std::cout << "recieved load stage grid" << std::endl;
        LoadStageData(message);
        return;
    } else if (message[2] == msg_signature) {
        ParsePlayerReadyRequest(message);
    }
    std::array<uint8_t, 32> response;
    response[0] = msg_lobby;
    response[1] = clients.size();
    response[2] = player_ready[0];
    response[3] = player_ready[1];
    response[4] = player_ready[2];
    response[5] = player_ready[3];
    response[6] = msg_signature;
    response[7] = msg_from_server;
    response[8] = msg_end;
    BroadcastMessage(response);
}

void ChangeGameState(){
    size_t players_ready = 0;
    for (int i = 0; i < 4; i++){
        if (player_ready[i]) players_ready++;
    }
    if (clients.size() == players_ready){
        if (loading_stage_phase == 0) {
            std::array<uint8_t, 32> response;
            response[0] = msg_lobby;
            response[1] = msg_load_stage_grid;
            response[2] = 0;
            response[3] = msg_from_server;
            response[4] = msg_end;
            BroadcastMessage(response);
            loading_stage_phase++;
            return;
        }
        if (loading_stage_phase > 1) {
            std::cout << "changing game state" << std::endl;
            std::array<uint8_t, 32> response;
            response[0] = msg_stage_data;
            response[1] = msg_end_stage_data;
            response[2] = msg_switch_to_game;
            response[3] = msg_signature;
            response[4] = msg_from_server;
            response[5] = msg_end;
            BroadcastMessage(response);
            InitGameState(&game_state);
        }
    }
}

void ProcessPlayerFC() {
    int fc_delay = 6;
    for (int i = 0; i < 4; i++){
        if (game_state.player_states[i] == PUNCH || game_state.player_states[i] == KICK){
            player_fcs[i].fc++;
            if (player_fcs[i].fc >= fc_delay){
                player_fcs[i].fc = 0;
                player_fcs[i].anim_fc++;
            }
        } else {
            player_fcs[i].fc = 0;
            player_fcs[i].anim_fc = 0;
        }
    }
}

Rectangle GeneratePlayerHitboxPunch(Rectangle rect, int face){
    Rectangle hitbox;
    uint16_t x;
    uint16_t w;
    uint16_t y = rect.y;
    uint16_t h = rect.height;

    if (face > 0){ // facing right
        x = uint16_t((float)rect.x + ((float)(rect.width) / 2.0f)); 
        w = uint16_t(((float)rect.width * 0.75f));
    } else {
        x = uint16_t((float)rect.x - ((float)(rect.width) / 4)); 
        w = uint16_t((float)rect.width * 0.75f);
    }

    hitbox = {x, y, w, h};
    return hitbox;
}

Rectangle GeneratePlayerHitboxKick(Rectangle rect, float scale, int face){
    Rectangle hitbox;
    uint16_t x;
    uint16_t w;
    uint16_t y = rect.y;
    uint16_t h = rect.height;

    if (face > 0){ // facing right
        x = uint16_t((float)rect.x + ((float)(rect.width) / 2.0f)); 
        w = uint16_t(((float)rect.width * 0.75f) + (scale * 2.0f));
    } else {
        x = uint16_t((float)rect.x - ((float)(rect.width) / 4) - (scale * 2.0f)); 
        w = uint16_t((float)rect.width * 0.75f);
    }

    hitbox = {x, y, w, h};
    return hitbox;
}


void UpdatePlayerHurtboxes(float scale, int player_width, int player_height){
    for (size_t i = 0; i < 4; i++){
        player_hurtboxes[i] = PlayerPosToRect(game_state.player_positions[i], scale, player_width, player_height);
    }
}

void ProcessPlayerAttacks(float scale) {
    for (size_t i = 0; i < 4; i++){
        if (i > clients.size()) break;
        if (game_state.player_states[i] == MOVE_RIGHT) player_faces[i] = 1;
        if (game_state.player_states[i] == MOVE_LEFT) player_faces[i] = -1;

        switch (PlayerState(game_state.player_states[i])){
            case PUNCH:
                player_hitboxes[i] = GeneratePlayerHitboxPunch(player_hurtboxes[i], player_faces[i]);
                if (player_fcs[i].anim_fc > 2) {
                    for (size_t j = 0; j < 4; j++){
                        if (i == j) continue;
                        if (RectRectCollision(player_hitboxes[i], player_hurtboxes[j]) && !processed_hit[i]){
                            std::cout << "player " << i << " hit player " << j << " with punch" << std::endl;
                            game_state.player_hps[j] -= 10;
                            processed_hit[i] = true;
                        }
                    }
                }
                break;
            case KICK:
                player_hitboxes[i] = GeneratePlayerHitboxKick(player_hurtboxes[i], scale, player_faces[i]);
                if (player_fcs[i].anim_fc > 5) {
                    for (size_t j = 0; j < 4; j++){
                        if (i == j) continue;
                        if (RectRectCollision(player_hitboxes[i], player_hurtboxes[j]) && !processed_hit[i]){
                            std::cout << "player " << i << " hit player " << j << " with kick" << std::endl;
                            game_state.player_hps[j] -= 10;
                            processed_hit[i] = true;
                        }
                    }
                }
                break;
            default:
                processed_hit[i] = false;
                break;

            }
        }
}

// this function is meant to be unique to the client,
// i.e. the client's request should only be changing
// their own player state, it should never affect
// the state of the other players, that is the 
// responsibility of the server 
void ParseGameStateRequest(std::array<uint8_t, 28>& current_game_state, std::array<uint8_t, 32>& last_recieved_bytes, GameState& game_state, ServerStage& stage){
    GameState req;
    req.FromBytes(last_recieved_bytes);

    std::array<uint8_t, 32> tmp;
    tmp[0] = msg_update;
    for (int i = 0; i < 28; i++){
        tmp[i+1] = current_game_state[i];
    }
    tmp[29] = msg_end;
    GameState curr;
    curr.FromBytes(tmp);

    int sender_id = last_recieved_bytes[30];
    // state update
    if (req.player_states[sender_id] != curr.player_states[sender_id]){ 
        game_state.player_states[sender_id] = req.player_states[sender_id];
    }
    // position update
    if (req.player_positions[sender_id].x != curr.player_positions[sender_id].x || req.player_positions[sender_id].y != curr.player_positions[sender_id].y){ 
        game_state.player_positions[sender_id] = req.player_positions[sender_id];
    }

    // attack update
    ProcessPlayerFC();
    UpdatePlayerHurtboxes(stage.scale, stage.player_width, stage.player_height);
    ProcessPlayerAttacks(stage.scale);

     // hp update
    if (req.player_hps[sender_id] != curr.player_hps[sender_id]){
        game_state.player_hps[sender_id] = req.player_hps[sender_id];
    }
    if (!(stage.ProcessPlayerCollision(game_state.player_positions[sender_id])) && PlayerState(game_state.player_states[sender_id]) == IDLE) {
        game_state.player_states[sender_id] = AIRBORNE;
    }

    switch(PlayerState(game_state.player_states[sender_id])){
        case MOVE_RIGHT: game_state.player_positions[sender_id].x += 7; break;
        case MOVE_LEFT: game_state.player_positions[sender_id].x -= 7; break;
        case MOVE_UP: game_state.player_positions[sender_id].y -= 7; break;
        case MOVE_DOWN: game_state.player_positions[sender_id].y += 7; break;
        case AIRBORNE: game_state.player_positions[sender_id].y += 3; break;
        case IDLE:
            break;
        default:
            break;
    }
    for (Vector2int pos : game_state.player_positions){
        if (stage.ProcessPlayerCollision(pos)){
            game_state.player_positions[sender_id] = curr.player_positions[sender_id];
            if (PlayerState(game_state.player_states[sender_id]) == AIRBORNE){
                game_state.player_states[sender_id] = IDLE;
            }
        }    
    }
    
}

void ParsePlayerReadyRequest(std::array<uint8_t, 32>& message){
    if (message[0] != msg_lobby) return;
    if (message[1] == msg_player_ready){
        int id = message[3];
        if (player_ready[id] == false){
            player_ready[id] = true;
        } else {
            player_ready[id] = false;
        }
        std::cout << "Player " << int(message[3]) << " is ready: " << player_ready[message[3]] << std::endl;
    }
}


void InitGameState(GameState* game){
    if (game_running) return;
    std::cout << "init game" << std::endl;
    for (PlayerFC& pfc : player_fcs){
        pfc.anim_fc = 0;
        pfc.fc = 0;
    }
    game_running = true;
    for (uint8_t& player_state : game->player_states) {
        player_state = 0xBB;
    }
    for (uint8_t& player_hp : game->player_hps){
        player_hp = 0xCC;
    }
    for (uint8_t& player_id : game->player_ids) {
        player_id = 0xAA;
    }
    for (Vector2int& player_position : game->player_positions) {
        player_position = Vector2int{0xDDDD, 0xEEEE};
    }
}