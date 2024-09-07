#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <game_manager.hpp>
#include <helpers.hpp>

int main() {

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

    std::cout << std::to_string(game_state.player_positions[0].x) << std::endl;

    uint8_t buf = 1;

    ClientSendBytes(&conn, (void*)&buf, 1);

    InitWindow(800, 450, "client");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ParseGameState(&game_state, &conn, &client_player);

        if (IsKeyPressed(KEY_L)){            
            LogGameState(game_state);
        }

        client_player.PollInput();
        BeginDrawing();
            ClearBackground(DARKGRAY);
            client_player.Draw();
            DrawDebugInfo(game_state);
            
        EndDrawing();

    }

    std::cout << "Closing application" << std::endl;

    CloseWebSocket(&conn);
    CloseWindow();

    return 0;
}