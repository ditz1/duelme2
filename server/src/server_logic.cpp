#include <server_logic.hpp>
#include <game_state.hpp>

ServerStage stage;

void LogMessageReceived(std::array<uint8_t, 32>& message) {    
    if (!message.empty()) {
        switch(message[0]) {
            case msg_connect: std::cout << "CONNECT" << std::endl; break;
            case msg_disconnect: std::cout << "DISCONNECT" << std::endl; break;
            case msg_update: std::cout << "UPDATE" << std::endl; break;
            case msg_ping: std::cout << "PING" << std::endl; break;
            case msg_lobby: std::cout << "LOBBY" << std::endl; break;
            case msg_player_ready: std::cout << "PLAYER_READY" << std::endl; break;
            case msg_switch_to_game: std::cout << "SWITCH_TO_GAME" << std::endl; break;
            case msg_load_stage_grid: std::cout << "LOAD_STAGE_GRID" << std::endl; break;
            case msg_stage_data: std::cout << "STAGE_DATA" << std::endl; break;
            default: std::cout << "UNKNOWN" << std::endl; break;
        }
    }

    std::cout << "Received message (" << message.size() << " bytes):" << std::endl;
    for (uint8_t c : message) {
        printf("%x | ", c);
    }
    printf("\n");
}

void ParseMessageReceived(std::array<uint8_t, 32>& message) {
    if (!message.empty()) {
        switch(message[0]) {
            case msg_connect:
                //LogMessageReceived(message);
                SendBackPlayerId(num_connections);
                UpdateLobbyState(message);
                break;
            case msg_disconnect:
                break;
            case msg_update:
                //LogMessageReceived(message);
                UpdateGameState(message);
                break;
            case msg_ping:
                std::cout << "PING" << std::endl;
                //LogMessageReceived(message);
                SendBackPlayerId(num_connections);
                break;
            case msg_stage_data:
                std::cout << "STAGE DATA" << std::endl;
                ParseSerialStageData(message, stage);
                //LogMessageReceived(message);
                break;
            case msg_lobby:
                std::cout << "LOBBY" << std::endl;
                //LogMessageReceived(message);
                UpdateLobbyState(message);
                ChangeGameState();
                break;
            default:
                std::cout << "Unknown message" << std::endl;
                std::cout << "Message: " << std::endl;
                for (uint8_t x : message){
                    printf("%x | ", x);
                }
                break;
        }
    }
}

void SendToClient(std::shared_ptr<websocket::stream<tcp::socket>> client, std::array<uint8_t, 32> message) {
    try {
        client->binary(true);
        client->write(boost::asio::buffer(message));
    } catch (const std::exception& e) {
        std::cerr << "Error sending message to client " << ": " << e.what() << std::endl;
        auto it = std::find(clients.begin(), clients.end(), client);
        std::remove(clients.begin(), clients.end(), clients.at(it - clients.begin()));         
    }
    
}

void BroadcastMessage(std::array<uint8_t, 32>& message) {
    
    for (auto& client : clients) {
        SendToClient(client, message);
        //std::cout << "broadcasting " << message.size() << " bytes" << std::endl;
    }
}

void ReAssignPlayerIds() {
    for (size_t i = 0; i < clients.size(); i++) {
        SendBackPlayerId(i);
    }
    for (size_t i = 3; i > clients.size(); i--){
        game_state.player_ids[i] = 0xAA;
        game_state.player_positions[i].x = 0xDDDD;
        game_state.player_positions[i].y = 0xEEEE;
        game_state.player_states[i] = 0xBB;
        game_state.player_hps[i] = 0xCC;
    }
    player_ready = {false, false, false, false};
}

void SendBackPlayerId(size_t client_id) {
    if (client_id >= clients.size()) return;

    std::array<uint8_t,32> msg; 
    msg[0] = msg_assign_id; 
    msg[1] = static_cast<uint8_t>(client_id);
    msg[2] = msg_end;
    game_state.player_ids[client_id] = client_id;
    switch (client_id){
        case 0:
            game_state.player_positions[client_id].x = 200;
            game_state.player_positions[client_id].y = 200;
            break;
        case 1:
            game_state.player_positions[client_id].x = 400;
            game_state.player_positions[client_id].y = 200;
            break;
        case 2:
            game_state.player_positions[client_id].x = 200;
            game_state.player_positions[client_id].y = 400;
            break;
        case 3:
            game_state.player_positions[client_id].x = 400;
            game_state.player_positions[client_id].y = 400;
            break;
        default:
            break;
    }
    
    //for (uint8_t x : msg){
    //    printf("%x | ", x);
    //}
    // printf("\n");
    SendToClient(clients[client_id], msg);
}
