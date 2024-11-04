#include <player_anims.hpp>

void AnimatePlayer(Player& player) {
    // if (player.IsAnimating()){            
    //     std::cout << player.texs[player.current_anim].fc << " | " <<
    //     player.fc << " | " << player.anim_current_frame << " | " << player.buffer_offset << " | " << player.fc_delay << std::endl;
    // }
    
    DrawTexture(*(player.tex), player.Position().x, player.Position().y, RAYWHITE);
    
}

// NOTE: all of these anim functions could be a template 👀 
// also, _is_animating is kind of a misnomer since it would always be animating,
// _is_animating being set to false would mean a new anim was triggered or an anim ended

// these functions are meant to contain drawing logic, no game logic should be here







///////////////////////////////////
////// these are all unused ///////
///////////////////////////////////






void AnimatePlayerPunch(Player& player){
    if (player.texs[player.current_anim].fc == 0){
        player.AssignTexture(PlayerState::PUNCH);
    } else {
        DrawCircle(player.Position().x, player.Position().y, 20, PURPLE);
        DrawTexture(*(player.tex), player.Position().x, player.Position().y, RAYWHITE);
    }
}
void AnimatePlayerKick(Player& player){
    if (player.texs[player.current_anim].fc == 0){
    } else {
        DrawCircle(player.Position().x, player.Position().y, 20, PURPLE);        
        DrawTexture(*(player.tex), player.Position().x, player.Position().y, RAYWHITE);
    }
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