#include <player.hpp>

Player::Player() {
    _requested_state = 0;
    _state = IDLE;
    _position = {400, 225};
}

Player::~Player() {

}

void Player::PollInput() {
    if (IsKeyDown(KEY_D)){
        _requested_state = 1;
        return;
    } else if (IsKeyDown(KEY_A)){
        _requested_state = 2;
        return;
    } else if (IsKeyDown(KEY_W)){
        _requested_state = 3;
        return;
    } else if (IsKeyDown(KEY_S)){
        _requested_state = 4;
        return;
    } else {
        _requested_state = 0;
    }
}

PlayerState Player::State() {
    return _state;
}

PlayerState Player::RequestedState() {
    return (PlayerState)_requested_state;
}


void Player::Draw() {
    DrawCircleV(_position, 20, RED);
}

int Player::Id() {
    return _id;
}
