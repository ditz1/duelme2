#include <s_player.hpp>
// 0 = no collision, 1 = left, 2 = right, 3 = top, 4 = bottom

static int gravity = 7;
static int velocity = 7;



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

