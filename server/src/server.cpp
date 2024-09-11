#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "../inc/s_protocol.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

std::vector<std::shared_ptr<websocket::stream<tcp::socket>>> clients;
std::mutex clients_mutex; // Mutex to protect the clients vector

void InitGameState(GameState* game){
    std::cout << "init game" << std::endl;
    game_running = true;
    for (uint8_t& player_state : game->player_states) {
        player_state = 0x6;
    }
    for (uint8_t& player_id : game->player_ids) {
        player_id = 0xAA;
    }
    for (Vector2int& player_position : game->player_positions) {
        player_position = Vector2int{0x7FFF, 0x7FFF};
    }
}

void LogMessageReceived(const std::vector<uint8_t>& message) {    
    if (!message.empty()) {
        switch(message[0]) {
            case msg_connect: std::cout << "CONNECT" << std::endl; break;
            case msg_disconnect: std::cout << "DISCONNECT" << std::endl; break;
            case msg_update: std::cout << "UPDATE" << std::endl; break;
            default: std::cout << "UNKNOWN" << std::endl; break;
        }
    }

    std::cout << "Received message (" << message.size() << " bytes):" << std::endl;
    for (uint8_t c : message) {
        printf("%x | ", c);
    }
    printf("\n");
}

void SendToClient(std::shared_ptr<websocket::stream<tcp::socket>> client, const std::vector<uint8_t>& message) {
    std::cout << message.size() << " bytes to send: " << std::endl;
    for (uint8_t x : message){
        printf("%x | ", x);
    }
    printf("\n");
    try {
        client->binary(true);
        client->write(boost::asio::buffer(message));
    } catch (const std::exception& e) {
        std::cerr << "Error sending message to client " << ": " << e.what() << std::endl;
        clients.erase(std::find(clients.begin(), clients.end(), client));
    }
    
}

void SendBackPlayerId(size_t client_index) {
    std::vector<uint8_t> msg = {msg_assign_id, static_cast<uint8_t>(client_index), msg_end};
    game_state.player_ids[client_index] = client_index;
    for (uint8_t x : msg){
        printf("%x | ", x);
    }
    printf("\n");
    SendToClient(clients[client_index], msg);
}

void BroadcastMessage(const std::vector<uint8_t>& message) {
    for (auto& client : clients) {
        SendToClient(client, message);
        std::cout << "broadcasting " << message.size() << " bytes" << std::endl;
    }
}

void UpdateGameState(const std::vector<uint8_t>& message) {
    //std::vector<uint8_t> msg;
    //msg.push_back(msg_update);
    //std::array<uint8_t, 26> bytes_msg;
    //std::cout << "updating" << std::endl;
    //for (int i = 0; i < 26; i++) {
    //    bytes_msg[i] = message[i];
    //}
    //printf("\n");
    //
    //game_state.FromBytes(bytes_msg);
    //std::array<uint8_t, 24> bytes = game_state.ToBytes();
    //std::cout << "to game state" << std::endl;
    //for (int i=0; i < 24; i++) {
    //    msg.push_back(bytes[i]);
    //}
    //printf("\n");
    //msg.push_back(msg_end);
    BroadcastMessage(message);
}

void ParseMessageReceived(const std::vector<uint8_t>& message) {
    if (!message.empty()) {
        switch(message[0]) {
            case msg_connect:
                LogMessageReceived(message);
                SendBackPlayerId(clients.size() - 1);
                break;
            case msg_disconnect:
                break;
            case msg_update:
                LogMessageReceived(message);
                UpdateGameState(message);
                break;
            case msg_ping:
                std::cout << "PING" << std::endl;
                SendBackPlayerId(clients.size() - 1);
                break;
            default:
                std::cout << "Unknown message" << std::endl;
                break;
        }
    }
}



//------------------------------------------------------------------------------

void StartSession(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        // Accept the websocket handshake
        ws->accept();

        // Add the client to the clients vector
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(ws);
        }

        for(;;) {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message
            ws->read(buffer);
            std::vector<uint8_t> message;
            for (size_t i = 0; i < buffer.size(); i++){
                message.push_back(boost::asio::buffer_cast<const uint8_t*>(buffer.data())[i]);
            }

            // Parse the received message
            ParseMessageReceived(message);

            // Echo the message back (if needed)
            ws->write(buffer.data());
        }
    }
    catch (const beast::system_error& se) {
        if (se.code() != websocket::error::closed) {
            std::cerr << "WebSocket error: " << se.code().message() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Remove the client from the clients list when the session ends
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
            [ws](const std::shared_ptr<websocket::stream<tcp::socket>>& client_ws) {
                return client_ws == ws;
            }), clients.end());
    }
}
//------------------------------------------------------------------------------

int main()
{
    InitGameState(&game_state);
    try
    {
        // Check command line arguments.
        auto const address = net::ip::make_address("192.168.1.42");
        auto const port = static_cast<unsigned short>(std::atoi("9000"));

        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        std::cout << "listening on " << address << ":" << port << std::endl;
        
        for (;;) {
            // This will receive the new connection
            tcp::socket socket{ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Create a shared pointer for the WebSocket stream
            auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));

            // Launch the session in a new thread
            std::thread(&StartSession, ws).detach();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}