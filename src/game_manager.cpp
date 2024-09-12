#include <game_manager.hpp>

void InitGameState(GameState* game){
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
        player_position = Vector2int{0xEEEE, 0xFFFF};
    }
}

void RequestStateUpdate(GameState* game, Connection* conn, Player* player) {
    if (int(player->RequestedState()) <= int(PlayerState::IDLE) && (game->player_states[int(player->Id())] != uint8_t(player->RequestedState()))) {
        game->player_states[int(player->Id())] = uint8_t(player->RequestedState());
        SendGameStateRequest(game, conn);
        std::cout << "Requesting State Update" << std::endl;
    }
}

void ParseAssignPlayerId(GameState* game, Connection* conn, Player* player){
    if (!conn->connected || data_from_server.size() == 0) return;
    if (num_players_connected == 4) {
        std::cout << "Server Full" << std::endl;
        return;
    }
    num_players_connected++;
    player->SetId(data_from_server[1]);
    game->player_ids[data_from_server[1]] = data_from_server[1];
    game->player_positions[data_from_server[1]].x = 200;
    game->player_positions[data_from_server[1]].y = 200;
    this_client_id = int(data_from_server[1]);
}

void UpdateGameState(GameState* game, Connection* conn){
    if (data_from_server.size() < 32) return;
    std::array<uint8_t, 32> last_received_bytes;
    for (size_t i = 0; i < 32; i++) {
        last_received_bytes[i] = data_from_server[i];
    }
    printf("\n");
    data_from_server.clear();
    GameState new_game_state;
    new_game_state.FromBytes(last_received_bytes);
    for (int i = 0; i < 4; i++){
        game->player_states[i] = new_game_state.player_states[i];
        game->player_positions[i] = new_game_state.player_positions[i];
        game->player_hps[i] = new_game_state.player_hps[i];
    }
}

void ParseGameState(GameState* game, Connection* conn, Player* player) {
    if (!conn->connected) return;
    if (data_from_server.size() < 1) return;

    std::cout << "recieved from server" <<  std::endl;
    printf("[ %x ]\n", data_from_server[0]);
    for (int i = 0; i < int(data_from_server.size()); i++) {
        printf("%x | ", data_from_server[i]);
    }
    printf("\n");
    switch (data_from_server[0]){
        case msg_connect:
            std::cout << "Connected To Server" << std::endl;
            break;
        case msg_disconnect:
            std::cout << "Disconnected From Server" << std::endl;
            break;
        case msg_assign_id:
            std::cout << "Assigned ID: " << int(data_from_server[1]) << std::endl;
            ParseAssignPlayerId(game, conn, player);
            break;
        case msg_update:
            std::cout << "Update" << std::endl;
            UpdateGameState(game, conn);
            break;
    }
    player->SetHp(game->player_hps[int(player->Id())]);
    player->SetState(PlayerState(game->player_states[int(player->Id())]));
    player->SetPosition((game->player_positions[int(player->Id())]));
}

void SendGameStateRequest(GameState* game, Connection* conn) {
    if (!conn->connected) return;
    std::array<uint8_t, 32> bytes_to_send;
    bytes_to_send[0] = msg_update;
    std::array<uint8_t, 28> game_bytes = game->ToBytes();
    for (int i = 0; i < 28; i++) {
        bytes_to_send[i+1] = game_bytes[i];
    }
    std::cout << "requesting new game state" << std::endl;
    bytes_to_send[29] = msg_end;    
    bytes_to_send[30] = 0x00;
    bytes_to_send[31] = 0x00;
    for (int i = 0; i < 32; i++) {
        printf("%x | ", bytes_to_send[i]);
    }
    printf("\n");
    ClientSendBytes(conn, (void*)&bytes_to_send, 32);
}