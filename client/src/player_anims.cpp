#include <player_anims.hpp>

void AnimatePlayer(Player& player) {
    if (player.IsAnimating()){
        //std::cout << "state " <<  player.State() << std::endl;
        //std::cout << "fc " << player.anim_frame_counter << std::endl;
    }
    switch(player.State()){
      case PUNCH:
          AnimatePlayerPunch(player);
          break;
      case KICK:
          AnimatePlayerKick(player);
          break;
      case JUMP:
          AnimatePlayerJump(player);
          break;
      case BLOCK:
          AnimatePlayerBlock(player);
          break;
      case MOVE_RIGHT:
          AnimatePlayerMoveRight(player);
          break;
      case MOVE_LEFT:
          AnimatePlayerMoveLeft(player);
          break;
      case IDLE:
          AnimatePlayerIdle(player);
          break;
      default:
          break;
    }
    // if (player.anim_frame_counter >= 60){
    //         player.anim_frame_counter = 0;
    //         player.SetIsAnimating(false);
    // }   
}

// NOTE: all of these anim functions could be a template 👀 
// also, _is_animating is kind of a misnomer since it would always be animating,
// _is_animating being set to false would mean a new anim was triggered or an anim ended

// these functions are meant to contain drawing logic, no game logic should be here

void AnimatePlayerPunch(Player& player){
    if (player.anim_frame_counter == 0){
    } else {
        DrawCircle(player.Position().x, player.Position().y, 20, PURPLE);
    }
}
void AnimatePlayerKick(Player& player){
    if (player.anim_frame_counter == 0){
    } else {
        DrawCircle(player.Position().x, player.Position().y, 20, PURPLE);        
    }
}

void AnimatePlayerJump(Player& player){
    if (player.anim_frame_counter == 0){
        //std::cout << "player jump" << std::endl;
        // jump logic will be way more complicated than anything else
        // should probably not confine it to this
    } else {
       
    }
}
void AnimatePlayerBlock(Player& player){
    if (player.anim_frame_counter == 0){
    } else {
        DrawCircle(player.Position().x, player.Position().y, 20, PURPLE);
    }
}
void AnimatePlayerMoveRight(Player& player){
    if (player.anim_frame_counter == 0){
    } else {
        
    }
}
void AnimatePlayerMoveLeft(Player& player){
    if (player.anim_frame_counter == 0){
    } else {
        
    }
}   
void AnimatePlayerIdle(Player& player){
    if (player.anim_frame_counter == 0){
    } else {
       
    } 
}