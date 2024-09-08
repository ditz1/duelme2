#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "../inc/s_protocol.hpp"
#include "../inc/server_logic.hpp"
#include "../inc/game_state.hpp"
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

namespace websocket = boost::beast::websocket;
using tcp = boost::asio::ip::tcp;

std::vector<std::shared_ptr<websocket::stream<tcp::socket>>> clients;
std::mutex clients_mutex;

void LogMessageReceived(const std::vector<uint8_t>& message) {    
    if (!message.empty()) {
        switch(message[0]) {
            case xCONNECT: std::cout << "CONNECT" << std::endl; break;
            case xDISCONNECT: std::cout << "DISCONNECT" << std::endl; break;
            case xUPDATE: std::cout << "UPDATE" << std::endl; break;
            default: std::cout << "UNKNOWN" << std::endl; break;
        }
    }

    std::cout << "Received message (" << message.size() << " bytes):" << std::endl;
    for (uint8_t c : message) {
        printf("%x ", c);
    }
    std::cout << std::endl;
}

void SendToClient(size_t client_index, const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    std::cout << message.size() << " bytes to send: " << std::endl;
    if (client_index < clients.size()) {
        try {
            clients[client_index]->write(boost::asio::buffer(message));
        } catch (const std::exception& e) {
            std::cerr << "Error sending message to client " << client_index << ": " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Invalid client index: " << client_index << std::endl;
    }
}

void SendBackPlayerId(size_t client_index) {
    std::vector<uint8_t> msg = {xASSIGN_ID, static_cast<uint8_t>(client_index), xEND_MSG};
    SendToClient(client_index, msg);
}

void BroadcastMessage(const std::vector<uint8_t>& message) {
    for (int i = 0; i < int(clients.size()); i++) {
        SendToClient(i, message);
        std::cout << "broadcasting " << message.size() << " bytes" << std::endl;
    }
}

void UpdateGameState(const std::vector<uint8_t>& message) {
    std::vector<uint8_t> msg;
    msg.push_back(xUPDATE);
    for (int i = 1; i < 17; i++) {
        msg.push_back(message[i]);
    }
    msg.push_back(xEND_MSG);
    BroadcastMessage(msg);
}

void ParseMessageReceived(const std::vector<uint8_t>& message) {
    if (!message.empty()) {
        switch(message[0]) {
            case xCONNECT:
                LogMessageReceived(message);
                SendBackPlayerId(clients.size() - 1);
                break;
            case xDISCONNECT:
                break;
            case xUPDATE:
                LogMessageReceived(message);
                UpdateGameState(message);
                break;
            default:
                std::cout << "Unknown message" << std::endl;
                break;
        }
    }
}



void HandleWebSocketMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        ws->accept();
        std::vector<uint8_t> buffer(32); // Adjust size as needed
        for(;;) {
            size_t bytes_read = ws->read_some(boost::asio::buffer(buffer));
            if (bytes_read > 0) {
                std::cout << "Received " << bytes_read << " bytes" << std::endl;
            }
            std::vector<uint8_t> message(buffer.begin(), buffer.begin() + bytes_read);
            ParseMessageReceived(message);
            buffer.clear();
        }
    }
    catch(boost::beast::system_error const& se) {
        if(se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch(std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), ws), clients.end());
}

int main() {
    try {
        boost::asio::io_context ioc;
        tcp::acceptor acceptor(ioc, {boost::asio::ip::make_address("192.168.1.42"), 9000});

        std::cout << "Server listening on port 9000" << std::endl;

        for(;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
            
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(ws);
            }
            
            std::thread(&HandleWebSocketMessage, ws).detach();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}