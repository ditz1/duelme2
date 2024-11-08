#include <s_player.hpp>

void PlayerMoveRight(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.x += 7;
    pos.y += 7; // gravity
    if (stage.ProcessPlayerCollision(pos)){
        pos.y -= 7;
        if (stage.ProcessPlayerCollision(pos)){
            pos.x -= 7;
            game_state.player_positions[id] = pos;
            return;
        } else {
            game_state.player_positions[id] = pos;
            return;
        }
    }
    game_state.player_positions[id] = pos;
}

void PlayerMoveLeft(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.x -= 7;
    pos.y += 7; // gravity
    if (stage.ProcessPlayerCollision(pos)){
        pos.y -= 7;
        if (stage.ProcessPlayerCollision(pos)){
            pos.x += 7;
            game_state.player_positions[id] = pos;
            return;
        } else {
            game_state.player_positions[id] = pos;
            return;
        }
    }
    game_state.player_positions[id] = pos;
}

void PlayerMoveUp(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.y -= 7;
    if (stage.ProcessPlayerCollision(pos)){
        pos.y += 7;
        game_state.player_positions[id] = pos;
    } else {
        game_state.player_positions[id] = pos;
    }
}

void PlayerMoveDown(ServerStage& stage, int id) {
    Vector2int pos = game_state.player_positions[id];
    pos.y += 7;
    if (stage.ProcessPlayerCollision(pos)){
        pos.y -= 7;
        game_state.player_positions[id] = pos;
    } else {
        game_state.player_positions[id] = pos;
    }
}

