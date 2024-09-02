#include "globals.hpp"
#include "networking.hpp"
#include <iostream>
#include <string>

int main() {
    Connection conn = {0};  // Create a Connection object, not a pointer
    OpenWebSocket(&conn, "ws://localhost:9000/ws");

    if (conn.ws <= 0) {
        std::cout << "Failed to create websocket" << std::endl;
        return 1;
    }

    // Wait for the connection to be established
    while (!conn.connected) {
        emscripten_sleep(10); // Small delay to avoid busy waiting
    }

    uint8_t buf = 1;

    ClientSendBytes(conn.ws, (void*)&buf, 1);

    InitWindow(800, 450, "client");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ONE)){
            ClientSendBytes(conn.ws, (void*)&buf, 1);
        }
        if (conn.last_received->numBytes > 0) {
            std::cout << "Received message of size: " << conn.last_received->numBytes << std::endl;
            for (int i = 0; i < conn.last_received->numBytes; i++) {
                printf("0x%x\n", conn.last_received->data[i]);
            }
            conn.last_received->numBytes = 0;
        }
        
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Client", 30, 30, 20, RAYWHITE);
        EndDrawing();
    }

    std::cout << "Closing application" << std::endl;

    CloseWindow();
    CloseWebSocket(conn.ws);

    return 0;
}