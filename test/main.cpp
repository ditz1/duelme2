#include "raylib.h"

int main() {
    InitWindow(800, 450, "Raylib Template");
    SetTargetFPS(60);

    Vector3 pos = { 0.0f, 0.0f, 0.0f };

    Camera3D cam = { 0 };
    cam.target = (Vector3){ 0.0f, 0.0f, 0.0f }; // Focus point of the camera
    cam.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position in 3D space
    cam.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Up direction of the camera
    cam.fovy = 45.0f; // Field of view in degrees
    cam.projection = CAMERA_PERSPECTIVE; // Use perspective projection

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_W)) cam.position.x += 0.1f;
        if (IsKeyDown(KEY_S)) cam.position.x -= 0.1f;
        if (IsKeyDown(KEY_A)) cam.position.z -= 0.1f;
        if (IsKeyDown(KEY_D)) cam.position.z += 0.1f;
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(cam);

                // Draw a red cube at the specified position
                DrawCube(pos, 2.0f, 2.0f, 2.0f, RED);
                DrawGrid(10, 1.0f); // Optional: Draw a grid for better visual reference

            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
