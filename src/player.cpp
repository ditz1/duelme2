#include <player.hpp>

Player::Player() {
    _requested_state = 0;
    _state = IDLE;
    _position = {400, 225};
}

Player::~Player() {

}

Vector2int Player::Position() {
    return _position;
}

void Player::PollInput() {
    int reset_state_flag = 0;
    if (IsKeyDown(KEY_D)){
        _requested_state = uint8_t(MOVE_RIGHT);
    } else {
        reset_state_flag++;
    }   

    if (IsKeyDown(KEY_A)){
        _requested_state = uint8_t(MOVE_LEFT);
    } else {
        reset_state_flag++;
    }

    if (IsKeyDown(KEY_W)){
        _requested_state = uint8_t(MOVE_UP);
    } else {
        reset_state_flag++;
    }

    if (IsKeyDown(KEY_S)){
        _requested_state = uint8_t(MOVE_DOWN);
    } else {
        reset_state_flag++;
    }

    if (reset_state_flag == uint8_t(IDLE)) {
        _requested_state = uint8_t(IDLE);
    }
}

PlayerState Player::State() {
    return _state;
}

PlayerState Player::RequestedState() {
    return PlayerState(_requested_state);
}


void Player::Draw() {
    DrawCircleV(V2intToV2(_position), 20, RED);
}

uint8_t Player::Id() {
    return _id;
}
