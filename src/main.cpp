#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <game_manager.hpp>
#include <helpers.hpp>


int main() {
    

    InitWindow(800, 450, "client");
    SetTargetFPS(60);

    GameState game_state;
    InitGameState(&game_state);

    Player client_player;
    Connection conn;
    OpenWebSocket(&conn, "ws://192.168.1.42:9000/ws");
    client_player.SetId(0);

    
    if (conn.ws <= 0) {
        std::cout << "Failed to create websocket" << std::endl;
        return 1;
    }

    while (!conn.connected) {
        emscripten_sleep(10); 
    }

    uint8_t buf = msg_connect;

    ClientSendBytes(&conn, (void*)&buf, 1);


    uint8_t msg[3] = {msg_ping, 0x00, msg_end};

    while (!WindowShouldClose()) {
        ParseGameState(&game_state, &conn, &client_player);


        if (IsKeyPressed(KEY_L)){            
            LogGameState(game_state, &conn);
        }
        if (IsKeyPressed(KEY_P)){
            std::cout << "ping" << std::endl;
            ClientSendBytes(&conn, (void*)msg, 3);
        }
        client_player.PollInput();
        RequestStateUpdate(&game_state, &conn, &client_player);        
        BeginDrawing();
            ClearBackground(DARKGRAY);
            client_player.Draw();
            DrawDebugInfo(game_state, client_player);
            
        EndDrawing();

    }

    std::cout << "Closing application" << std::endl;

    CloseWebSocket(&conn);
    CloseWindow();

    return 0;
}