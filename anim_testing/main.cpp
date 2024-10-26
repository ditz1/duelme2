#include "raylib.h"
#include <string>

////// input for testing //////

std::string filename = "stickman.gif";

///////////////////////////////







typedef class Dummy {
public:    
    Image img;
    Texture2D tex;
    int fc = 0;
    int anim_fc = 0;
    int anim_current_frame = 0;
    unsigned int buff_offset = 0;
    int fc_delay = 12;
} Dummy;

void UpdateDummyAnimation(Dummy* dummy){
    dummy->fc++;
    if (dummy->fc >= dummy->fc_delay)
    {
        // Move to next frame
        dummy->anim_current_frame++;
        if (dummy->anim_current_frame >= dummy->anim_fc) dummy->anim_current_frame = 0; // if final frame is reached we return to first frame
        // Get memory offset position for next frame data in image.data
        // this is 4 because single byte per channel (RGBA)
        dummy->buff_offset = dummy->img.width*dummy->img.height*4*dummy->anim_current_frame;
        // Update GPU texture data with next frame image data
        // WARNING: Data size (frame size) and pixel format must match already created texture
        // "void* pixels" is pointer to image raw data
        UpdateTexture(dummy->tex, ((unsigned char *)dummy->img.data) + dummy->buff_offset);
        dummy->fc = 0;
    }
}

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "anim testing");

    SetTargetFPS(60);

    Dummy dummy;

    // anim_fc must be set to zero prior to assigning image and texture to object
    // because these functions will overwrite fc value
    dummy.img = LoadImageAnim(filename.c_str(), &(dummy.anim_fc));
    dummy.tex = LoadTextureFromImage(dummy.img);
    
    bool play_anim = false;
    bool play_anim_once = false;

    while (!WindowShouldClose())   
    {        

        BeginDrawing();


        if (IsKeyDown(KEY_ONE)) {
            play_anim = true;
        } else {
            play_anim = false;
        };
        if (IsKeyPressed(KEY_TWO)) play_anim_once = true;
    
        if (play_anim_once && (dummy.anim_current_frame < dummy.anim_fc - 1)){
            UpdateDummyAnimation(&dummy);
        } else {
            play_anim_once = false;
            dummy.anim_current_frame = 0;
        }


        if (play_anim) UpdateDummyAnimation(&dummy);


        ClearBackground(RAYWHITE);

        DrawText("Anim Testing", 50, 50, 20, BLACK);
        DrawText(TextFormat("img offset: %d", dummy.buff_offset), 50, 80, 20, BLACK);
        DrawText(TextFormat("fc: %d", dummy.fc), 50, 110, 20, BLACK);
        DrawText(TextFormat("anim_fc: %d", dummy.anim_fc), 50, 140, 20, BLACK);
        
        DrawTexture(dummy.tex, GetScreenWidth()/2 - dummy.tex.width/2, 140, WHITE);

        EndDrawing();
    }

    UnloadTexture(dummy.tex);
    UnloadImage(dummy.img);

    CloseWindow();
}
