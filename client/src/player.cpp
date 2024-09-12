#include <player.hpp>

Player::Player() {
    _requested_state = 0;
    _state = IDLE;
    _position = {0xEEEE, 0xDDDD};
}

Player::~Player() {

}

Vector2int Player::Position() {
    return _position;
}

void Player::PollInput() {
    if (IsKeyDown(KEY_D)){
        _requested_state = uint8_t(MOVE_RIGHT);
        return;
    }   
    if (IsKeyDown(KEY_A)){
        _requested_state = uint8_t(MOVE_LEFT);
        return;
    } 
    if (IsKeyDown(KEY_W)){
        _requested_state = uint8_t(MOVE_UP);
        return;
    } 

    if (IsKeyDown(KEY_S)){
        _requested_state = uint8_t(MOVE_DOWN);
        return;
    } 

    if (IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S)){
        _requested_state = uint8_t(IDLE);
        return;
    }
    
}

PlayerState Player::State() {
    return _state;
}

void Player::SetId(int id) {
    _id = id;
    switch(_id){
        case 0:
            _color = RED;
            break;
        case 1:
            _color = BLUE;
            break;
        case 2:
            _color = GREEN;
            break;
        case 3:
            _color = YELLOW;
            break;
    }   
}

PlayerState Player::RequestedState() {
    return PlayerState(_requested_state);
}


void Player::Draw() {
    DrawCircleV(V2intToV2(_position), 20, _color);
}

uint8_t Player::Id() {
    return _id;
}
