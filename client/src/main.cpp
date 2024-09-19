#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <game_manager.hpp>
#include <helpers.hpp>

int current_game_stage = 0;

int main() {
    
    InitWindow(800, 450, "client");
    SetTargetFPS(60);
    
    GameState game_state;
    InitGameState(&game_state);

    Player client_player;
    std::array<Player, 4> all_players = {Player(), Player(), Player(), Player()};
    Connection conn;
    OpenWebSocket(&conn, "ws://192.168.1.42:9000/ws");

    
    if (conn.ws <= 0) {
        std::cout << "Failed to create websocket" << std::endl;
        return 1;
    }

    while (!conn.connected) {
        emscripten_sleep(10); 
    }

    std::array<uint8_t, 32> buf;
    buf[0] = msg_connect;
    ClientSendBytes(&conn, (void*)buf.data(), 32);


    buf[0] = msg_ping;
    buf[1] = 0x00;
    buf[2] = msg_end;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_V)){
            developer_mode = !developer_mode;
        }

        switch (current_game_stage){
            case 0:
                ParseAssignPlayerId(&game_state, &conn, &client_player);
                if (IsKeyPressed(KEY_SPACE)){
                    SendReadyRequest(&client_player, &conn);
                }
                ParseLobbyState(&game_state);
                break;
            case 1:
                ParseGameState(&game_state, &conn, &client_player);
                break;
            case 2:
                ParseEndState(&game_state, &conn, &client_player);
                break;

        }


        if (IsKeyPressed(KEY_L)){            
            LogGameState(game_state, &conn);
        }
        if (IsKeyPressed(KEY_P)){
            std::cout << "ping" << std::endl;
            ClientSendBytes(&conn, (void*)buf.begin(), 32);
        }
        client_player.PollInput();
        RequestStateUpdate(&game_state, &conn, &client_player);        
        BeginDrawing();
            ClearBackground(DARKGRAY);
            //client_player.Draw();
            switch(current_game_stage){
                case 0:
                    DrawLobbyState(&game_state);
                    break;
                case 1:
                    DrawGameState(&game_state);
                    break;
                case 2:
                    DrawText("Game Over", 400, 225, 20, RED);
                    break;
            }
            DrawDebugInfo(game_state, client_player);
            
        EndDrawing();

    }

    buf[0] = msg_disconnect;
    ClientSendBytes(&conn, (void*)&buf, 32);

    std::cout << "Closing application" << std::endl;

    CloseWebSocket(&conn);
    CloseWindow();

    return 0;
}