#include "raylib.h"
#include <string>
#include <array>

////// input for testing //////

std::string filename = "assets/red/red_idle.gif";

///////////////////////////////

typedef struct AnimData {
    Image img;
    Texture2D tex;
    int fc = 0;
} AnimData;

typedef class Dummy {
public:    
    Image* img;
    Texture2D* tex;
    int fc = 0;
    int anim_current_frame = 0;
    unsigned int buff_offset = 0;
    int fc_delay = 12;
    int current_anim = 0;
    std::array<AnimData, 7> anims;
    bool anims_loaded = false;

} Dummy;

void LoadDummyAnimations(Dummy& dummy){
    dummy.anims[0].img = LoadImageAnim("assets/red/red_idle.gif", &(dummy.anims[0].fc));
    dummy.anims[0].tex = LoadTextureFromImage(dummy.anims[0].img);
    dummy.anims[1].img = LoadImageAnim("assets/red/red_walk.gif", &(dummy.anims[1].fc));
    dummy.anims[1].tex = LoadTextureFromImage(dummy.anims[1].img);
    dummy.anims[2].img = LoadImageAnim("assets/red/red_punch.gif", &(dummy.anims[2].fc));
    dummy.anims[2].tex = LoadTextureFromImage(dummy.anims[2].img);
    dummy.anims[3].img = LoadImageAnim("assets/red/red_kick.gif", &(dummy.anims[3].fc));
    dummy.anims[3].tex = LoadTextureFromImage(dummy.anims[3].img);
    dummy.anims[4].img = LoadImageAnim("assets/red/red_jump.gif", &(dummy.anims[4].fc));
    dummy.anims[4].tex = LoadTextureFromImage(dummy.anims[4].img);
    dummy.anims[5].img = LoadImageAnim("assets/red/red_die.gif", &(dummy.anims[5].fc));
    dummy.anims[5].tex = LoadTextureFromImage(dummy.anims[5].img);
    dummy.anims[6].img = LoadImageAnim("assets/red/red_enterblock.gif", &(dummy.anims[6].fc));
    dummy.anims[6].tex = LoadTextureFromImage(dummy.anims[6].img);
}

void UpdateDummyAnimation(Dummy* dummy){
    dummy->fc++;
    if (dummy->fc >= dummy->fc_delay)
    {
        // Move to next frame
        dummy->anim_current_frame++;
        if (dummy->anim_current_frame >= dummy->anims[dummy->current_anim].fc) dummy->anim_current_frame = 0; // if final frame is reached we return to first frame
        // Get memory offset position for next frame data in image.data
        // this is 4 because single byte per channel (RGBA)
        dummy->buff_offset = dummy->img->width*dummy->img->height*4*dummy->anim_current_frame;
        // Update GPU texture data with next frame image data
        // WARNING: Data size (frame size) and pixel format must match already created texture
        // "void* pixels" is pointer to image raw data
        UpdateTexture(*(dummy->tex), ((unsigned char *)dummy->img->data) + dummy->buff_offset);
        dummy->fc = 0;
    }
}


void FlipImages(Dummy& dummy){
    for (int i = 0; i < 7; i++){
        ImageFlipHorizontal(&(dummy.anims[i].img));
        UnloadTexture((dummy.anims[i].tex));
        dummy.anims[i].tex = LoadTextureFromImage(dummy.anims[i].img);
    }
}
int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "anim testing");

    SetTargetFPS(60);

    Dummy dummy;
    LoadDummyAnimations(dummy);
    // anim_fc must be set to zero prior to assigning image and texture to object
    // because these functions will overwrite fc value

    dummy.current_anim = 0;
    dummy.img = &dummy.anims[dummy.current_anim].img;
    dummy.tex = &dummy.anims[dummy.current_anim].tex;


    bool play_anim = false;
    bool play_anim_once = false;

    while (!WindowShouldClose())   
    {        

        BeginDrawing();

        if (IsKeyPressed(KEY_LEFT)){
            dummy.current_anim--;
            if (dummy.current_anim < 0) dummy.current_anim = 6;
            dummy.img = &dummy.anims[dummy.current_anim].img;
            dummy.tex = &dummy.anims[dummy.current_anim].tex;
            dummy.anim_current_frame = 0;
        }
        if (IsKeyPressed(KEY_RIGHT)){
            dummy.current_anim++;
            if (dummy.current_anim > 6) dummy.current_anim = 0;
            dummy.img = &dummy.anims[dummy.current_anim].img;
            dummy.tex = &dummy.anims[dummy.current_anim].tex;
            dummy.anim_current_frame = 0;
        }


        if (IsKeyDown(KEY_ONE)) {
            play_anim = true;
        } else {
            play_anim = false;
        };
        if (IsKeyPressed(KEY_TWO)) play_anim_once = true;
    
        if (play_anim_once && (dummy.anim_current_frame < dummy.anims[dummy.current_anim].fc - 1)){
            UpdateDummyAnimation(&dummy);
        } else {
            play_anim_once = false;
            dummy.anim_current_frame = 0;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            FlipImages(dummy);
        } 

        if (play_anim) UpdateDummyAnimation(&dummy);


        ClearBackground(RAYWHITE);

        DrawText("Anim Testing", 50, 50, 20, BLACK);
        DrawText(TextFormat("img offset: %d", dummy.buff_offset), 50, 80, 20, BLACK);
        DrawText(TextFormat("fc: %d", dummy.fc), 50, 110, 20, BLACK);
        DrawText(TextFormat("anim_fc: %d", dummy.anims[dummy.current_anim].fc), 50, 140, 20, BLACK);
        DrawTextureEx(*(dummy.tex), {400.0f, 300.0f}, 0.0f, 4.0f, WHITE);
        DrawTexture(*(dummy.tex), GetScreenWidth()/2 - (dummy.tex->width)/2, 140, WHITE);

        EndDrawing();
    }

    UnloadTexture(*(dummy.tex));
    UnloadImage(*(dummy.img));

    CloseWindow();
}
