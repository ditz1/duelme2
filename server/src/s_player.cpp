#include <s_player.hpp>
// 0 = no collision, 1 = left, 2 = right, 3 = top, 4 = bottom

static int gravity = 7;
static int velocity = 10;



void PlayerMoveRight(CollisionIndex dirs, Vector2int& pos) {
    pos.x += velocity;
    if (!dirs[6] && !dirs[7]) {
        pos.y += 5;
    }
}

void PlayerMoveLeft(CollisionIndex dirs, Vector2int& pos) {
    pos.x -= velocity;
    if (!dirs[6] && !dirs[7]) {
        pos.y += 5;
    }
}

void PlayerMoveUp(CollisionIndex dirs, Vector2int& pos) {
    pos.y -= velocity;
}

void PlayerMoveDown(CollisionIndex dirs, Vector2int& pos) {
    pos.y += velocity;
}

void PlayerIdle(CollisionIndex dirs, Vector2int& pos) {
    pos.y += gravity;
}

