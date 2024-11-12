#include <player_anims.hpp>

void AnimatePlayer(Player& player) {
    // if (player.IsAnimating()){            
    //     std::cout << player.texs[player.current_anim].fc << " | " <<
    //     player.fc << " | " << player.anim_current_frame << " | " << player.buffer_offset << " | " << player.fc_delay << std::endl;
    // }
    switch(player.State()){
        case PlayerState::PUNCH:
            AnimatePlayerPunch(player);
            break;
        case PlayerState::SHOOT:
            AnimatePlayerShoot(player);
            break;
        case PlayerState::KICK:
            AnimatePlayerKick(player);
            break;
        default:
            break;
    }
        
}

// NOTE: all of these anim functions could be a template 👀 
// also, _is_animating is kind of a misnomer since it would always be animating,
// _is_animating being set to false would mean a new anim was triggered or an anim ended

// these functions are meant to contain drawing logic, no game logic should be here



///////////////////////////////////
////// these are all unused /////// * mainly for testing
///////////////////////////////////



void AnimatePlayerPunch(Player& player){
    float scale = player.draw_data.scale;
    Rectangle hitbox;
    if (player.FaceDir() > 0){ // facing right
        float x = (float)player.Bounds().x + ((float)(player.Bounds().width) / 2); 
        float y = (float)player.Bounds().y;
        float w = ((float)player.Bounds().width * 0.75f);
        float h = (float)player.Bounds().height;
        hitbox = {x, y, w, h};
    } else {
        float x = (float)player.Bounds().x - ((float)(player.Bounds().width) / 4); 
        float y = (float)player.Bounds().y;
        float w = ((float)player.Bounds().width * 0.75f);
        float h = (float)player.Bounds().height;
        hitbox = {x, y, w, h};
    }
    DrawRectangleLinesEx(hitbox, 3, YELLOW);

}
void AnimatePlayerKick(Player& player){
    float scale = player.draw_data.scale;

    Rectangle hitbox;
    float x;
    float w;
    float y = (float)player.Bounds().y;
    float h = (float)player.Bounds().height;

    if (player.FaceDir() > 0){ // facing right
        x = (float)player.Bounds().x + ((float)(player.Bounds().width) / 2); 
        w = ((float)player.Bounds().width * 0.75f) + (scale * 2.0f);
    } else {
        x = (float)player.Bounds().x - ((float)(player.Bounds().width) / 4) - (scale * 2.0f); 
        w = ((float)player.Bounds().width * 0.75f);
    }

    hitbox = {x, y, w, h};

    DrawRectangleLinesEx(hitbox, 3, YELLOW);
}

void AnimatePlayerShoot(Player& player){
    float scale = player.draw_data.scale;

    Rectangle hitbox;
    float x;
    float w;
    float y = (float)player.Bounds().y;
    float h = (float)player.Bounds().height;

    int shot_dist = 50;

    if (player.FaceDir() > 0){ // facing right
        x = (float)player.Bounds().x + ((float)(player.Bounds().width) / 2); 
        w = ((float)player.Bounds().width * 0.75f) + (scale * 2.0f) + shot_dist;
    } else {
        x = (float)player.Bounds().x - ((float)(player.Bounds().width) / 4) - (scale * 2.0f) - shot_dist; 
        w = ((float)player.Bounds().width * 0.75f) + shot_dist;
    }

    hitbox = {x, y, w, h};

    DrawRectangleLinesEx(hitbox, 3, YELLOW);
}

void AnimatePlayerJump(Player& player){
    if (player.texs[player.current_anim].fc == 0){
        //std::cout << "player jump" << std::endl;
        // jump logic will be way more complicated than anything else
        // should probably not confine it to this
    } else {
        DrawTexture(*(player.tex), player.Position().x, player.Position().y, RAYWHITE);
    }
}
void AnimatePlayerBlock(Player& player){
    if (player.texs[player.current_anim].fc == 0){
    } else {
        DrawCircle(player.Position().x, player.Position().y, 20, PURPLE);
        DrawTexture(*(player.tex), player.Position().x, player.Position().y, RAYWHITE);
    }
}
void AnimatePlayerMoveRight(Player& player){
    if (player.texs[player.current_anim].fc == 0){
    } else {
        
    }
}
void AnimatePlayerMoveLeft(Player& player){
    if (player.texs[player.current_anim].fc == 0){
        
    }
}   
void AnimatePlayerIdle(Player& player){
    if (player.texs[player.current_anim].fc == 0){
    } else {
        DrawTexture(*(player.tex), player.Position().x, player.Position().y, RAYWHITE);
    } 
}