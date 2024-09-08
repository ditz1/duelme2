#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "../inc/s_protocol.hpp"
#include <iostream>
#include <vector>
#include <mutex>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

std::vector<std::shared_ptr<websocket::stream<tcp::socket>>> clients;
std::mutex clients_mutex;

void LogMessageReceived(const std::vector<uint8_t>& message) {    
    if (!message.empty()) {
        switch(message[0]) {
            case xCONNECT: 
                std::cout << "CONNECT" << std::endl; 
                break;
            case xDISCONNECT: 
                std::cout << "DISCONNECT" << std::endl; 
                break;
            case xUPDATE: 
                std::cout << "UPDATE" << std::endl; 
                break;
            default: 
                std::cout << "UNKNOWN" << std::endl;
                break;
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
            clients[client_index]->write(asio::buffer(message));
        } catch (const std::exception& e) {
            std::cerr << "Error sending message to client " << client_index << ": " << e.what() << std::endl;
        }
        
    } else {
        std::cerr << "Invalid client index: " << client_index << std::endl;
    }
}

void SendBackPlayerId(size_t client_index) {
    std::vector<uint8_t> msg;
    msg.push_back(xASSIGN_ID);
    msg.push_back(client_index);
    msg.push_back(xEND_MSG);
    SendToClient(client_index, msg);
}

void ParseMessageReceived(const std::vector<uint8_t>& message) {
    std::cout << "size: " << message.size() << std::endl;
    if (!message.empty()) {
        switch(message[0]) {
            case xCONNECT:
                LogMessageReceived(message);
                SendBackPlayerId(clients.size() - 1);
                break;
            case xDISCONNECT:
                LogMessageReceived(message);
                break;
            case xUPDATE:
                LogMessageReceived(message);
                break;
            default:
                std::cout << "Unknown message" << std::endl;
                break;
        }
    }
}

void BroadcastMessage(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients) {
        try {
            client->write(asio::buffer(message));
        } catch (const std::exception& e) {
            std::cerr << "Error broadcasting message: " << e.what() << std::endl;
        }
    }
}


void HandleWebSocketMessage(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        ws->accept();
        for(;;) {
            beast::flat_buffer buffer;
            ws->read(buffer);
            
            std::vector<uint8_t> message;
            message.reserve(buffer.size());

            auto data = buffer.data();
            const uint8_t* begin = static_cast<const uint8_t*>(data.data());
            const uint8_t* end = begin + data.size();
            message.insert(message.end(), begin, end);
            
            ParseMessageReceived(message);
        }
    }
    catch(beast::system_error const& se) {
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
        asio::io_context ioc;
        tcp::acceptor acceptor(ioc, {asio::ip::make_address("192.168.1.42"), 9000});

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