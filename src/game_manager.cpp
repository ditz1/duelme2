#include <game_manager.hpp>

void InitGameState(GameState* game){
    for (uint8_t& player_state : game->player_states) {
        player_state = uint8_t(PlayerState::UNREGISTERED);
    }
    for (uint8_t player_id : game->player_ids) {
        player_id = 0xFF;
    }
    for (Vector2int& player_position : game->player_positions) {
        player_position = Vector2int{0x7FFF, 0x7FFF};
    }
}

void RequestStateUpdate(GameState* game, Connection* conn, Player* player) {
    //if (conn->last_received->data[0] != msg_update) return;
    if (int(player->RequestedState()) <= int(PlayerState::IDLE) && (game->player_states[int(player->Id())] != uint8_t(player->RequestedState()))) {
        game->player_states[int(player->Id())] = uint8_t(player->RequestedState());
        SendGameStateRequest(game, conn);
    }
}

void ParseAssignPlayerId(GameState* game, Connection* conn, Player* player){
    player->SetId(int(conn->last_received->data[1]));
    game->player_ids[int(conn->last_received->data[1])] = int(conn->last_received->data[1]);
    this_client_id = int(conn->last_received->data[1]);
}

void UpdateGameState(GameState* game, Connection* conn){
    std::array<uint8_t, 26> last_received_bytes;
    std::cout << "recieved from server" <<  std::endl;
    for (int i = 0; i < conn->last_received->numBytes; i++) {
        last_received_bytes[i] = conn->last_received->data[i];
        printf("%x | ", conn->last_received->data[i]);
    }
    printf("\n");
    game->FromBytes(last_received_bytes);

}

void ParseGameState(GameState* game, Connection* conn, Player* player) {
    if (!conn->connected) return;
    if (conn->last_received->numBytes < 1) return;
    std::cout << "recieved from server" <<  std::endl;
    printf("[ %x ] ", conn->last_received->data[0]);
    for (int i = 0; i < conn->last_received->numBytes; i++) {
        printf("%x | ", conn->last_received->data[i]);
    }
    switch (conn->last_received->data[0]){
        case msg_connect:
            std::cout << "Connected To Server" << std::endl;
            break;
        case msg_disconnect:
            std::cout << "Disconnected From Server" << std::endl;
            break;
        case msg_assign_id:
            std::cout << "Assigned ID: " << int(conn->last_received->data[1]) << std::endl;
            ParseAssignPlayerId(game, conn, player);
            break;
        case msg_update:
            std::cout << "Update" << std::endl;
            UpdateGameState(game, conn);
            break;
    }

    //LogGameState(*game);
    
    // if (game->player_states[player->Id() != uint8_t(player->RequestedState())]) {
    //     game->player_states[player->Id()] = uint8_t(player->RequestedState());
    //     for (int i = 0; i < sizeof(game->ToBytes()); i++) {
    //         printf(" %x |", (game->ToBytes())[i]);
    //     }
    // }
    //SendGameState(game, conn);
    conn->last_received->numBytes = 0;
}

void SendGameStateRequest(GameState* game, Connection* conn) {
    if (!conn->connected) return;
    uint8_t bytes_to_send[game->ToBytes().size() + 2];
    bytes_to_send[0] = msg_update;
    std::array<uint8_t, 24> game_bytes = game->ToBytes();
    for (int i = 1; i < game_bytes.size(); i++) {
        bytes_to_send[i] = game_bytes[i-1];
    }
    bytes_to_send[25] = msg_end;
    ClientSendBytes(conn, (void*)&bytes_to_send, 26);
}