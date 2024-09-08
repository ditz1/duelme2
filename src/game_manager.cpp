#include <game_manager.hpp>

void InitGameState(GameState* game){
    for (uint8_t& player_state : game->player_states) {
        player_state = uint8_t(PlayerState::UNREGISTERED);
    }
    for (uint8_t player_id : game->player_ids) {
        player_id = 0xAA;
    }
    for (Vector2int& player_position : game->player_positions) {
        player_position = Vector2int{0x7FFF, 0x7FFF};
    }
}

void RequestStateUpdate(GameState* game, Connection* conn, Player* player) {
    if (int(player->RequestedState()) < int(PlayerState::IDLE)){
        std::cout << "Requesting State Update" << std::endl;
        game->player_states[int(player->Id())] = uint8_t(player->RequestedState());
        SendGameState(game, conn);
    }
}

void ParseGameState(GameState* game, Connection* conn, Player* player) {
    if (!conn->connected) return;
    if (conn->last_received->numBytes < 1) return;

    std::cout << "num bytes from server: " << int(conn->last_received->numBytes) << std::endl;

    for (int i = 0; i < conn->last_received->numBytes; i++) {
        printf("%x | ", conn->last_received->data[i]);
    }
    printf("\n");

    switch (conn->last_received->data[0]){
        case xCONNECT:
            std::cout << "Connected To Server" << std::endl;
            break;
        case xDISCONNECT:
            std::cout << "Disconnected From Server" << std::endl;
            break;
        case xASSIGN_ID:
            std::cout << "Assigned ID: " << int(conn->last_received->data[1]) << std::endl;
            player->SetId(int(conn->last_received->data[1]));
            game->player_ids[int(conn->last_received->data[1])] = int(conn->last_received->data[1]);
            this_client_id = int(conn->last_received->data[1]);
            break;
        case xUPDATE:
            std::cout << "Update" << std::endl;
            break;
    }

    LogGameState(*game);
    
    if (game->player_states[player->Id() != uint8_t(player->RequestedState())]) {
        game->player_states[player->Id()] = uint8_t(player->RequestedState());
        for (int i = 0; i < sizeof(game->ToBytes()); i++) {
            printf(" %x |", (game->ToBytes())[i]);
        }
    }
    SendGameState(game, conn);
    conn->last_received->numBytes = 0;
}

void SendGameState(GameState* game, Connection* conn) {
    if (!conn->connected) return;
    uint8_t bytes_to_send[18];
    bytes_to_send[0] = xUPDATE;
    std::array<uint8_t, 16> game_bytes = game->ToBytes();
    for (int i = 1; i < game_bytes.size(); i++) {
        bytes_to_send[i] = game_bytes[i];
    }
    bytes_to_send[17] = xEND_MSG;
    std::cout << "sending" << std::endl;
    for (int i = 0; i < sizeof(bytes_to_send); i++) {
        printf(" %x |", bytes_to_send[i]);
    }
    ClientSendBytes(conn, (void*)&bytes_to_send, 18);
}