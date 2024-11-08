#include <s_player.hpp>
// 0 = no collision, 1 = left, 2 = right, 3 = bottom, 4 = top

static int gravity = 7;
static int velocity = 7;

// // top collision
//     if (std::get<3>(collision)) {
//         std::cout << "top collision" << std::endl;
//         return;
//     }
//     // bottom collision
//     if (std::get<2>(collision)) {
//         std::cout << "bottom collision" << std::endl;
//         return;
//     }
//     // right collision
//     if (std::get<0>(collision)) {
//         std::cout << "right collision" << std::endl;
//         return;
//     }
//     // left collision
//     if (std::get<1>(collision)) {
//         std::cout << "left collision" << std::endl;
//         return;
//     }


void PlayerMoveRight(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.x += velocity;
    std::tuple<bool,bool,bool,bool> collision = stage.ProcessPlayerCollisionDirection(pos);
    if (!std::get<0>(collision) && !std::get<1>(collision) && !std::get<2>(collision) && !std::get<3>(collision)) {
        game_state.player_positions[id] = pos;
        return;
    }
    if (std::get<0>(collision)) pos.x -= velocity;

    game_state.player_positions[id] = pos;
}

void PlayerMoveLeft(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.x -= velocity;
    std::tuple<bool,bool,bool,bool> collision = stage.ProcessPlayerCollisionDirection(pos);
    if (!std::get<0>(collision) && !std::get<1>(collision) && !std::get<2>(collision) && !std::get<3>(collision)) {
        game_state.player_positions[id] = pos;
        return;
    }

    if (std::get<1>(collision)) pos.x += velocity;
 

    game_state.player_positions[id] = pos;

}

void PlayerMoveUp(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.y -= velocity;
    std::tuple<bool,bool,bool,bool> collision = stage.ProcessPlayerCollisionDirection(pos);
    if (!std::get<0>(collision) && !std::get<1>(collision) && !std::get<2>(collision) && !std::get<3>(collision)) {
        game_state.player_positions[id] = pos;
        return;
    }

    if (std::get<3>(collision)) pos.y += 7;

    game_state.player_positions[id] = pos;
   
}

void PlayerMoveDown(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.y += velocity;
     std::tuple<bool,bool,bool,bool> collision = stage.ProcessPlayerCollisionDirection(pos);
    if (!std::get<0>(collision) && !std::get<1>(collision) && !std::get<2>(collision) && !std::get<3>(collision)) {
        game_state.player_positions[id] = pos;
        return;
    }

    if (std::get<2>(collision)) pos.y -= 7;

    game_state.player_positions[id] = pos;

  
}

