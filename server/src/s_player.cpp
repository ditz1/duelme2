#include <s_player.hpp>
// 0 = no collision, 1 = left, 2 = right, 3 = top, 4 = bottom

float max_x_velocity = 2.0f;
float max_y_velocity = 2.0f; 
float max_x_acceleration = 0.5f;
float max_y_acceleration = 0.5f;
int max_distance = 20;

void PlayerApplyPhysics(Vector2int& pos, PlayerBody& body) {
    float last_pos_x = body.pos_x;
    float last_pos_y = body.pos_y;
    
    body.pos_x = 2.0f * body.pos_x - body.last_pos_x + body.acc_x * dt * dt;
    body.pos_y = 2.0f * body.pos_y - body.last_pos_y + body.acc_y * dt * dt;

    
    body.last_pos_x = last_pos_x;
    body.last_pos_y = last_pos_y;
    
    body.vel_x = (body.pos_x - body.last_pos_x) / dt;
    body.vel_y = (body.pos_y - body.last_pos_y) / dt;
    

    // if (body.vel_x > max_x_velocity) body.vel_x = max_x_velocity;
    // if (body.vel_x < -max_x_velocity) body.vel_x = -max_x_velocity;
    // if (body.vel_y > max_y_velocity) body.vel_y = max_y_velocity;
    // if (body.vel_y < -max_y_velocity) body.vel_y = -max_y_velocity;
    
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

void PlayerIdle(PlayerBody& body) {
    body.acc_x = 0;
    body.acc_y = 0;
    body.vel_x = 0;
    body.vel_y = 0;
    body.last_pos_x = body.pos_x;
    body.last_pos_y = body.pos_y;

}

void PlayerMoveUp(CollisionIndex dirs, PlayerBody& body) {
    if (dirs[4] && dirs[5]) {
        return;
    }
    body.vel_y = -accel_const;
}


void PlayerMoveDown(CollisionIndex dirs, PlayerBody& body) {
    if (dirs[6] && dirs[7]) {
        return;
    }
    body.acc_y += accel_const;
}


