#include <s_player.hpp>
// 0 = no collision, 1 = left, 2 = right, 3 = top, 4 = bottom

float max_x_velocity = 2.0f;
float max_y_velocity = 2.0f; 
float max_x_acceleration = 0.5f;
float max_y_acceleration = 0.5f;
float max_distance = 5.0f;

void PlayerApplyPhysics(Vector2int& pos, PlayerBody& body) {
    const float p_last_pos_x = body.pos_x;
    const float p_last_pos_y = body.pos_y;

    float new_pos_x = 2.0f * body.pos_x - body.last_pos_x + body.acc_x * dt * dt;
    float new_pos_y = 2.0f * body.pos_y - body.last_pos_y + body.acc_y * dt * dt;
    
    float x_diff = new_pos_x - body.pos_x;
    float y_diff = new_pos_y - body.pos_y;

    if (x_diff > max_distance) x_diff = max_distance;
    if (x_diff < -max_distance) x_diff = -max_distance;
    if (y_diff > max_distance) y_diff = max_distance;
    if (y_diff < -max_distance) y_diff = -max_distance;

    body.pos_x += x_diff;
    body.pos_y += y_diff;
    body.last_pos_x = p_last_pos_x;
    body.last_pos_y = p_last_pos_y;
    
    body.acc_x = 0;
    body.acc_y = 0;
    
    pos.x = uint16_t(body.pos_x);
    pos.y = uint16_t(body.pos_y);
}

void PlayerApplyGravity(CollisionIndex dirs, PlayerBody& body) {
    bool airborne = !dirs[6] && !dirs[7];
    if (!airborne) return;
    body.acc_y += gravity;
}

void PlayerMoveRight(CollisionIndex dirs, PlayerBody& body) {
    if (dirs[0] && dirs[1]) {
        return;
    }
    body.acc_x += accel_const;
}

void PlayerMoveLeft(CollisionIndex dirs, PlayerBody& body) {
    if (dirs[2] && dirs[3]) {
        return;
    }
    body.acc_x -= accel_const;
}

void PlayerIdle(CollisionIndex dirs, PlayerBody& body) {
    body.acc_x = 0;
    body.acc_y = 0;
    body.vel_x = 0;
    body.vel_y = 0;
    //body.last_pos_x = body.pos_x;
    //body.last_pos_y = body.pos_y;    

}

void PlayerMoveUp(CollisionIndex dirs, PlayerBody& body, PlayerFC fc) {
    if (dirs[4] && dirs[5]) {
        return;
    }

    if (fc.anim_fc < 1) {
        body.last_pos_y -= 0.75f;
        body.pos_y -= 1.5f;
    }
}


void PlayerMoveDown(CollisionIndex dirs, PlayerBody& body) {
    if (dirs[6] && dirs[7]) {
        return;
    }
    body.acc_y += accel_const;
}


