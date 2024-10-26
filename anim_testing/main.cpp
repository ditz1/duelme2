#include "raylib.h"


int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "anim testing");

    SetTargetFPS(60);

    int anim_fc = 0;

    Image stickman = LoadImageAnim("stickman.gif", &anim_fc);
    Texture2D stickman_tex = LoadTextureFromImage(stickman);
    unsigned int buff_offset = 0;
    int fc = 0;
    int current_frame = 0;
    int fc_delay = 8;              

    while (!WindowShouldClose())   
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Anim Testing", 50, 50, 20, BLACK);

        EndDrawing();
    }
}
