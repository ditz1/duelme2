#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <iostream>
#include <string>

int main() {

    GameState game_state;
    
    Player player;
    player.GetConnection();
    Connection* conn = player.GetConnection();   // Create a Connection object, not a pointer
    OpenWebSocket(conn, "ws://192.168.1.42:9000/ws");
    int current_player_id = -1;

    if (conn->ws <= 0) {
        std::cout << "Failed to create websocket" << std::endl;
        return 1;
    }

    // Wait for the connection to be established
    while (!conn->connected) {
        emscripten_sleep(10); // Small delay to avoid busy waiting
    }

    uint8_t buf = 1;

    ClientSendBytes(conn->ws, (void*)&buf, 1);

    InitWindow(800, 450, "client");
    SetTargetFPS(60);
    
    // register player
    for (int i = 0; i < conn->last_received->numBytes; i++) {
        printf("0x%x | ", conn->last_received->data[i]);
    }
    printf("\n");

    while (!WindowShouldClose()) {
        player.Update();
        BeginDrawing();
            ClearBackground(DARKGRAY);
            player.Draw();
            DrawText("Client", 30, 30, 20, RAYWHITE);
            DrawText("- Player -", 30, 60, 20, RAYWHITE);
            DrawText(TextFormat("State: %s", (PlayerStateToString.at(player.State())).c_str()), 30, 90, 20, RAYWHITE);
            DrawText(TextFormat("Requested: %s", (PlayerStateToString.at(player.RequestedState())).c_str()), 30, 120, 20, RAYWHITE);
        EndDrawing();

    }

    std::cout << "Closing application" << std::endl;

    CloseWindow();
    CloseWebSocket(conn->ws);

    return 0;
}