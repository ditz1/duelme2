#include <game_state.hpp>
#include <chrono>
#include <thread>

std::array<CollisionIndex, 4> player_coll_dirs = {CollisionIndex{false, false, false, false, false, false, false, false},
                                                  CollisionIndex{false, false, false, false, false, false, false, false},
                                                  CollisionIndex{false, false, false, false, false, false, false, false},
                                                  CollisionIndex{false, false, false, false, false, false, false, false}};
std::array<Rectangle, 4> player_hitboxes;
std::array<Rectangle, 4> player_hurtboxes;
std::array<bool, 4> processed_hit;
std::array<bool, 4> player_airborne;
std::array<int, 4> player_faces;
std::array<PlayerBody, 4> player_bodies;
std::array<bool, 4> p_can_jump = {true, true, true, true};
std::array<bool, 4> p_restart = {false, false, false, false};
bool game_has_restarted = false;

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
    int can_restart = 0;
    for (int i = 0; i < 4; i++){
        if (p_restart[i]) can_restart++;
    }
    if (can_restart > 2){
        std::cout << "reset" << std::endl;
        ChangeGameState(true);
        p_restart = {false, false, false, false};
    }    
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
       

        loading_stage_phase = 2;
        ChangeGameState();
        std::cout << "recieved end stage data, starting game" << std::endl;
        return;
    }

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

void ChangeGameState(bool restart){
    size_t players_ready = 0;
    for (int i = 0; i < 4; i++){
        if (player_ready[i]) players_ready++;
    }
    if (restart) {
        std::cout << "restarting game" << std::endl;
        std::array<uint8_t, 32> response;
        response[0] = msg_update;
        response[1] = msg_reset_game;
        response[2] = msg_signature;
        response[3] = msg_from_server;
        response[4] = msg_end;
        BroadcastMessage(response);
        p_restart = {false, false, false, false};
        return;
    }
    if (clients.size() == players_ready){
        std::array<uint8_t, 32> response;
        switch(loading_stage_phase) {
            case 0:
                response[0] = msg_lobby;
                response[1] = msg_load_stage_grid;
                response[2] = 0;
                response[3] = msg_from_server;
                response[4] = msg_end;
                BroadcastMessage(response);
                loading_stage_phase++;
                break;
            case 2:
                std::cout << "changing game state" << std::endl;
                response[0] = msg_stage_data;
                response[1] = msg_end_stage_data;
                response[2] = msg_switch_to_game;
                response[3] = msg_signature;
                response[4] = msg_from_server;
                response[5] = msg_end;
                BroadcastMessage(response);
                InitGameState(&game_state);
                break;
        }
    }
}

void ProcessPlayerFC() {
    int fc_delay = 6;
    for (int i = 0; i < 4; i++){
        switch(game_state.player_states[i]){
            case PUNCH:
            case KICK:
            case SHOOT:
            case MOVE_UP:
                player_fcs[i].fc++;
                if (player_fcs[i].fc >= fc_delay){
                    player_fcs[i].fc = 0;
                    player_fcs[i].anim_fc++;
                }
                break;
            default:
                player_fcs[i].fc = 0;
                player_fcs[i].anim_fc = 0;
                break;
        }         
    }
}


void UpdatePlayerHurtboxes(float scale, int player_width, int player_height){
    for (size_t i = 0; i < 4; i++){
        player_hurtboxes[i] = PlayerPosToRect(game_state.player_positions[i], scale, player_width, player_height);
    }
}

void ProcessPlayerAttacks(float scale) {
    for (size_t i = 0; i < 4; i++) {
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
            case SHOOT:
                player_hitboxes[i] = GeneratePlayerHitboxShoot(player_hurtboxes[i], scale, player_faces[i]);
                if (player_fcs[i].anim_fc > 1) {
                    for (size_t j = 0; j < 4; j++){
                        if (i == j) continue;
                        if (RectRectCollision(player_hitboxes[i], player_hurtboxes[j]) && !processed_hit[i]){
                            std::cout << "player " << i << " hit player " << j << " with shoot" << std::endl;
                            game_state.player_hps[j] -= 50;
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

void ProcessPlayerPhysics() {
    // for (int i = 0; i < 4; i++){
    //     if (game_state.player_states[i] == AIRBORNE){
    //         game_state.player_positions[i].y += 2;
    //     }
    // }
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
        if (game_state.player_states[sender_id] == AIRBORNE && req.player_states[sender_id] == MOVE_UP){
            game_state.player_states[sender_id] = AIRBORNE;
        } else if(!p_can_jump[sender_id] && req.player_states[sender_id] == MOVE_UP) {
                game_state.player_states[sender_id] = AIRBORNE;
        }else {
            game_state.player_states[sender_id] = req.player_states[sender_id];
        }

    }
    ///// do not change this ///// was causing desync
    if ((req.player_positions[sender_id].x == curr.player_positions[sender_id].x) && (req.player_positions[sender_id].y == curr.player_positions[sender_id].y)){ 
        game_state.player_positions[sender_id] = req.player_positions[sender_id];
    }

    if (((int)req.player_hps[sender_id] == 0) || ((int)req.player_hps[sender_id] > 240)){
        game_state.player_hps[sender_id] = 0;
        curr.player_hps[sender_id] = 0;
    }

    if (((int)curr.player_hps[sender_id] == 0) || ((int)curr.player_hps[sender_id] > 240)){
        game_state.player_hps[sender_id] = 0;
    }

    if (game_state.player_hps[sender_id] == 0 || game_state.player_hps[sender_id] > 240){
        game_state.player_states[sender_id] = MOVE_DOWN;
    }

    for (int i = 0; i < 4; i++){
        if (game_state.player_hps[i] == 0 || game_state.player_hps[i] > 240 || game_state.player_states[i] == MOVE_DOWN){
            p_restart[i] = true;
        } else {
            p_restart[i] = false;
        }
        if (i > (clients.size() - 1)){
            p_restart[i] = true;
        }
    }



    
    // if position is updated here, it will cause desync
    if (((req.player_positions[sender_id].x != curr.player_positions[sender_id].x) || (req.player_positions[sender_id].y != curr.player_positions[sender_id].y)) && req.player_states[sender_id] != AIRBORNE){
        std::cout << "desync" << std::endl;
        
    }
    ///////////////////////



    // attack update (do not put this out of scope)
    ProcessPlayerPhysics();
    ProcessPlayerFC();
    UpdatePlayerHurtboxes(stage.scale, stage.player_width, stage.player_height);
    ProcessPlayerAttacks(stage.scale);


     // hp update
    if (req.player_hps[sender_id] != curr.player_hps[sender_id]){
        game_state.player_hps[sender_id] = req.player_hps[sender_id];
    }
    // move + collision detection
    Vector2int pos = game_state.player_positions[sender_id];
    Vector2int old_pos = game_state.player_positions[sender_id];


    

    switch(PlayerState(game_state.player_states[sender_id])){
        case MOVE_RIGHT:
            PlayerMoveRight(player_coll_dirs[sender_id], player_bodies[sender_id]); 
            PlayerApplyGravity(player_coll_dirs[sender_id], player_bodies[sender_id]); 
            break;
        case MOVE_LEFT: 
            PlayerMoveLeft(player_coll_dirs[sender_id], player_bodies[sender_id]); 
            PlayerApplyGravity(player_coll_dirs[sender_id], player_bodies[sender_id]);
            break;
        case MOVE_UP:
            p_can_jump[sender_id] = false;
            PlayerMoveUp(player_coll_dirs[sender_id], player_bodies[sender_id], player_fcs[sender_id]); 
            break;
        case MOVE_DOWN: PlayerMoveDown(player_coll_dirs[sender_id], player_bodies[sender_id]);break;
        case PUNCH:
        case KICK:
        case SHOOT:
        case AIRBORNE:
            PlayerApplyGravity(player_coll_dirs[sender_id], player_bodies[sender_id]);
            break;
        case IDLE: 
            PlayerIdle(player_coll_dirs[sender_id], player_bodies[sender_id]);
            break;
        default: break;
    }

    PlayerApplyPhysics(pos, player_bodies[sender_id]);
    

    // check if position is colliding with anything
    bool coll = stage.ProcessPlayerCollision(pos);
    int spacing = (stage.min_y_level / 2) + 3;
    player_coll_dirs[sender_id] = stage.ProcessPlayerCollisionDirection(pos);
    bool right_coll = player_coll_dirs[sender_id][0] && player_coll_dirs[sender_id][1];
    bool left_coll = player_coll_dirs[sender_id][2] && player_coll_dirs[sender_id][3];
    bool top_coll = player_coll_dirs[sender_id][4] && player_coll_dirs[sender_id][5];
    bool bottom_coll = player_coll_dirs[sender_id][6] && player_coll_dirs[sender_id][7];
   
    //std::string s;
    //if (left_coll) s += " left ";
    //if (right_coll) s += " right ";
    //if (top_coll) s += " top ";
    //if (bottom_coll) s += " bottom ";
    //if (s.size() > 1)
    //    std::cout << "player " << sender_id << " collision: " << s << std::endl;

    if (coll && ((game_state.player_states[sender_id] != MOVE_RIGHT) && (game_state.player_states[sender_id] != MOVE_LEFT) && (game_state.player_states[sender_id] != MOVE_UP))){
        if (right_coll) pos.x -= 7;
        if (left_coll) pos.x += 7;
        if (top_coll) pos.y += 1;
        if (bottom_coll) pos.y -= 1;
        pos = old_pos;
        player_bodies[sender_id].last_pos_x = player_bodies[sender_id].pos_x;
        player_bodies[sender_id].last_pos_y = player_bodies[sender_id].pos_y;
        
    }   
    
    if (!bottom_coll && (game_state.player_states[sender_id] == IDLE)){
        game_state.player_states[sender_id] = AIRBORNE;
    }
    if (bottom_coll && (game_state.player_states[sender_id] == AIRBORNE)){
        game_state.player_states[sender_id] = IDLE;
    }
    if (bottom_coll) p_can_jump[sender_id] = true;

    if (game_state.player_states[sender_id] == MOVE_UP && player_fcs[sender_id].anim_fc > 2){
        game_state.player_states[sender_id] = AIRBORNE;
    }



    
    // apply movement if colliding
    switch(PlayerState(game_state.player_states[sender_id])){
        case MOVE_RIGHT: 
        case MOVE_LEFT:
        case AIRBORNE: 
            if (bottom_coll) {
                pos.y -= gravity;
                p_can_jump[sender_id] = true;
            }
            break;
        case MOVE_UP: break;
        case MOVE_DOWN: break;
        case IDLE: break;
        default: break;
    }

    
    player_coll_dirs[sender_id] = stage.ProcessPlayerCollisionDirection(pos);
    right_coll = player_coll_dirs[sender_id][0] && player_coll_dirs[sender_id][1];
    left_coll = player_coll_dirs[sender_id][2] && player_coll_dirs[sender_id][3];
    top_coll = player_coll_dirs[sender_id][4] && player_coll_dirs[sender_id][5];
    bottom_coll = player_coll_dirs[sender_id][6] && player_coll_dirs[sender_id][7];


    // reassign position if colliding
    if (left_coll){
        pos.x += 3;
        player_bodies[sender_id].vel_x = 0;
        player_bodies[sender_id].acc_x = 0;
        player_bodies[sender_id].pos_x = pos.x;
        player_bodies[sender_id].last_pos_x = pos.x;
    }   
    if (right_coll) { 
        pos.x -= 3;
        player_bodies[sender_id].vel_x = 0;
        player_bodies[sender_id].acc_x = 0;
        player_bodies[sender_id].pos_x = pos.x; 
        player_bodies[sender_id].last_pos_x = pos.x;
    }
    if (top_coll){    
        pos.y += 1;
        player_bodies[sender_id].vel_y = 0;
        player_bodies[sender_id].acc_y = 0;
        player_bodies[sender_id].pos_y = pos.y;
        player_bodies[sender_id].last_pos_y = pos.y;
    }
    if (bottom_coll) {
        pos.y -= 1;
        player_bodies[sender_id].vel_y = 0;
        player_bodies[sender_id].acc_y = 0;
        player_bodies[sender_id].pos_y = pos.y;
        player_bodies[sender_id].last_pos_y = pos.y;
        p_can_jump[sender_id] = true;
    }

    if (left_coll && right_coll){
        pos.y -= 1;
    }

    if (pos.y < stage.min_y_level + spacing){ 
        pos.y = stage.min_y_level + spacing;
    }

    if (pos.y > stage.max_y_level - ((float)spacing * 2.2f)){
        pos.y = stage.max_y_level - ((float)spacing * 2.2f);
        player_bodies[sender_id].vel_y = 0;
        player_bodies[sender_id].acc_y = 0;
        player_bodies[sender_id].pos_y = pos.y;
        player_bodies[sender_id].last_pos_y = pos.y;
        p_can_jump[sender_id] = true;
        if (game_state.player_states[sender_id] == AIRBORNE){
            game_state.player_states[sender_id] = IDLE;
        }
        if (game_state.player_states[sender_id] == IDLE){
            player_bodies[sender_id].pos_x = pos.x;
            player_bodies[sender_id].last_pos_x = pos.x;
        }
    }

    if (pos.x < stage.min_x_level + spacing){ 
        pos.x = stage.min_x_level + spacing;
    }

    if (pos.x > stage.max_x_level - spacing) {
        pos.x = stage.max_x_level - spacing;
    }
    
    game_state.player_positions[sender_id] = pos;
    
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