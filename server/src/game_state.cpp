#include "../inc/game_state.hpp"

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
    std::cout << "sender id: " << sender_id << std::endl;
    
    // state update
    if (req.player_states[sender_id] != curr.player_states[sender_id]){ 
        game_state.player_states[sender_id] = req.player_states[sender_id];
    }
    // position update
    if (req.player_positions[sender_id].x != curr.player_positions[sender_id].x || req.player_positions[sender_id].y != curr.player_positions[sender_id].y){ 
        game_state.player_positions[sender_id] = req.player_positions[sender_id];
    }
    switch(game_state.player_states[sender_id]){
        case 0: game_state.player_positions[sender_id].x += 100; break;
        case 1: game_state.player_positions[sender_id].x -= 100; break;
        case 2: game_state.player_positions[sender_id].y -= 100; break;
        case 3: game_state.player_positions[sender_id].y += 100; break;
        case 4:
            break;
        default:
            break;
    }    
    // hp update
    if (req.player_hps[sender_id] != curr.player_hps[sender_id]){
        game_state.player_hps[sender_id] = req.player_hps[sender_id];
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