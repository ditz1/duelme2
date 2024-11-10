#include <s_player.hpp>
// 0 = no collision, 1 = left, 2 = right, 3 = top, 4 = bottom




void PlayerMoveRight(CollisionIndex dirs, Vector2int& pos) {
    pos.x += velocity;
    bool airborne = !dirs[6] && !dirs[7];
    if (airborne) {
        pos.y += gravity;
    } else {
        pos.y -= 2;
    }
}

void PlayerMoveLeft(CollisionIndex dirs, Vector2int& pos) {
    pos.x -= velocity;
    bool airborne = !dirs[6] && !dirs[7];
    if (airborne) {
        pos.y += gravity;
    } else {
        pos.y -= 2;
    }
}

void PlayerMoveUp(CollisionIndex dirs, Vector2int& pos) {
    if (dirs[4] && dirs[5]) {
        return;
    }
    pos.y -= velocity;
}

void PlayerMoveDown(CollisionIndex dirs, Vector2int& pos) {
    if (dirs[6] && dirs[7]) {
        return;
    }
    pos.y += velocity;
}


