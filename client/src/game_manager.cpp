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
        player_position = Vector2int{0xDDDD, 0xEEEE};
    }
}

void RequestStateUpdate(GameState* game, Connection* conn, Player* player) {
    if (int(player->RequestedState()) <= int(PlayerState::IDLE) && int(game->player_states[player->Id()]) != int(player->RequestedState())){
        game->player_states[int(player->Id())] = uint8_t(player->RequestedState());
        SendGameStateRequest(game, conn);
        std::cout << "Requesting State Update" << std::endl;
    }
}

void ParseAssignPlayerId(GameState* game, Connection* conn, Player* player){
    if (!conn->connected || data_from_server.size() == 0 || data_from_server[0] != msg_assign_id) return;
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
    conn->last_received->numBytes = 0;
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
    bytes_to_send[29] = msg_signature;    
    bytes_to_send[30] = this_client_id;
    bytes_to_send[31] = msg_end;
    for (int i = 0; i < 32; i++) {
        printf("%x | ", bytes_to_send[i]);
    }
    printf("\n");
    ClientSendBytes(conn, (void*)&bytes_to_send, 32);
}

void DrawGameState(GameState* game){
    for (int i = 0; i < 4; i++){
        Player player;
        player.SetId(i);
        player.SetState(PlayerState(game->player_states[i]));
        player.SetHp(game->player_hps[i]);
        player.SetPosition(game->player_positions[i]);
        player.Draw();
    }
}

void DrawLobbyScene(GameState* game) {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    bool player_ready[4] = 
        { game->player_states[0] == PlayerState::READY, 
          game->player_states[1] == PlayerState::READY, 
          game->player_states[2] == PlayerState::READY, 
          game->player_states[3] == PlayerState::READY };

    DrawRectangle(screen_width/2 - 400, screen_height/2 - 400, 200, 200, player_ready[0] ? GREEN : RED);
    DrawRectangle(screen_width/2 + 200, screen_height/2 - 400, 200, 200, player_ready[1] ? GREEN : RED);
    DrawRectangle(screen_width/2 - 400, screen_height/2 + 200, 200, 200, player_ready[2] ? GREEN : RED);
    DrawRectangle(screen_width/2 + 200, screen_height/2 + 200, 200, 200, player_ready[3] ? GREEN : RED);

}