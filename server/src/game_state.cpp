#include "../inc/game_state.hpp"
#include <chrono>
#include <thread>

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


void UpdateGameState(std::array<uint8_t, 32>& message) {
    if (message[0] != msg_update) return; 

    std::array<uint8_t, 32> response;    
    
    std::array<uint8_t, 28> curr_game_bytes = game_state.ToBytes();

    ParseGameStateRequest(curr_game_bytes, message, game_state);

    std::array<uint8_t, 28> updated_game_bytes = game_state.ToBytes();

    response[0] = msg_update;
    std::copy(updated_game_bytes.begin(), updated_game_bytes.end(), response.begin() + 1);
    response[29] = msg_end;
    response[30] = msg_from_server;
    response[31] = msg_end;
    BroadcastMessage(response);
}

void UpdateLobbyState(std::array<uint8_t, 32>& message) {
    if (message[0] != msg_lobby) return;
    if (message[2] == msg_signature) {
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
        std::array<uint8_t, 32> response;
        response[0] = msg_lobby;
        response[1] = msg_switch_to_game;
        response[2] = msg_signature;
        response[3] = msg_from_server;
        response[3] = msg_end;
        BroadcastMessage(response);
        InitGameState(&game_state);
    }
}

// this function is meant to be unique to the client,
// i.e. the client's request should only be changing
// their own player state, it should never affect
// the state of the other players, that is the 
// responsibility of the server 
void ParseGameStateRequest(std::array<uint8_t, 28>& current_game_state, std::array<uint8_t, 32>& last_recieved_bytes, GameState& game_state){
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
    switch(PlayerState(game_state.player_states[sender_id])){
        case MOVE_RIGHT: game_state.player_positions[sender_id].x += 7; break;
        case MOVE_LEFT: game_state.player_positions[sender_id].x -= 7; break;
        case MOVE_UP: game_state.player_positions[sender_id].y -= 7; break;
        case MOVE_DOWN: game_state.player_positions[sender_id].y += 7; break;
        case IDLE:
            break;
        default:
            break;
    }    
    // hp update
    if (req.player_hps[sender_id] != curr.player_hps[sender_id]){
        game_state.player_hps[sender_id] = req.player_hps[sender_id];
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