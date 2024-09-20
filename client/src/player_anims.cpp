#include <player_anims.hpp>

void AnimatePlayer(Player& player) {
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
}

void AnimatePlayerPunch(Player& player){
    if (player.anim_frame_counter == 0){
        std::cout << "player punch" << std::endl;
        player.anim_frame_counter++;
    } else {
        player.anim_frame_counter++;
        if (player.anim_frame_counter == 60){
            player.anim_frame_counter = 0;
            player.SetRequestedState(IDLE);
        }
    }
}
void AnimatePlayerKick(Player& player){
    if (player.anim_frame_counter == 0){
        std::cout << "player kick" << std::endl;
        player.anim_frame_counter++;
    } else {
        player.anim_frame_counter++;
        if (player.anim_frame_counter == 60){
            player.anim_frame_counter = 0;
            player.SetRequestedState(IDLE);
        }
    }
}
void AnimatePlayerJump(Player& player){
    if (player.anim_frame_counter == 0){
        std::cout << "player jump" << std::endl;
        player.anim_frame_counter++;
    } else {
        player.anim_frame_counter++;
        if (player.anim_frame_counter == 60){
            player.anim_frame_counter = 0;
            player.SetRequestedState(IDLE);
        }
    }
}
void AnimatePlayerBlock(Player& player){
    if (player.anim_frame_counter == 0){
        std::cout << "player block" << std::endl;
        player.anim_frame_counter++;
    } else {
        player.anim_frame_counter++;
        if (player.anim_frame_counter == 60){
            player.anim_frame_counter = 0;
            player.SetRequestedState(IDLE);
        }
    }
}
void AnimatePlayerMoveRight(Player& player){
    std::cout << "player move right" << std::endl;
}
void AnimatePlayerMoveLeft(Player& player){
    std::cout << "player move left" << std::endl;
}   
void AnimatePlayerIdle(Player& player){
    
}