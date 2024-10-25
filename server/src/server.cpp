#include "../inc/s_protocol.hpp"
#include "../inc/game_state.hpp"
#include "../inc/server_logic.hpp"


//------------------------------------------------------------------------------
void StartSession(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        // websocket handshake
        ws->accept();

        // add the client to the clients vector
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(ws);
            num_connections++;
            std::cout << "client connected -- " << clients.size() << " clients remain" << std::endl;
        }

        for(;;) {
           beast::flat_buffer buffer;
           ws->read(buffer);
           
           auto bytes = buffer.data();
           std::array<uint8_t, 32> message;
           
           if (buffer.size() == 32) {
               std::copy_n(static_cast<uint8_t*>(bytes.data()), 32, message.begin());
               ParseMessageReceived(message);
           } else {
                std::cout << "Invalid message size: " << buffer.size() << std::endl;
           }

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

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
            [ws](const std::shared_ptr<websocket::stream<tcp::socket>>& client_ws) {
                return client_ws == ws;
            }), clients.end());
        std::cout << "client disconnected -- " << clients.size() << " clients remain" << std::endl;
        ReAssignPlayerIds();
        num_connections--;
    }
}
//------------------------------------------------------------------------------

int main()
{
    InitGameState(&game_state);
    try
    {
        auto const address = net::ip::make_address("192.168.1.42");
        auto const port = static_cast<unsigned short>(std::atoi("9000"));

        // the io_context is required
        net::io_context ioc{1};

        // the acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        std::cout << "listening on " << address << ":" << port << std::endl;
        
        for (;;) {
            // receive the new connection
            boost::asio::any_io_executor ex = ioc.get_executor();
            tcp::socket socket{ex};

            // block until we get a connection
            acceptor.accept(socket);

            // create a shared pointer for the ws stream
            auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));

            // launch the session in a new thread
            std::thread(&StartSession, ws).detach();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}