#include <player.hpp>
#include <player_anims.hpp>

Player::Player() {
    _requested_state = 0;
    _state = IDLE;
    _position = {0xEEEE, 0xDDDD};
    _current_anim = 0;
    _is_animating = false;
    _player_face_dir = 1;
    anim_frame_counter = 0;
    _hp = 100;
}

Player::~Player() {

}

Vector2int Player::Position() {
    return _position;
}


void Player::ProcessPlayerAnimLogic() {
    if (_is_animating){
        anim_frame_counter++;
    }

    if (PlayerState(_requested_state) != _state) {
        anim_frame_counter = 0;
    }

    if (anim_frame_counter >= 60 || _is_animating == false){
        anim_frame_counter = 0;
        _is_animating = false;
    }
}

void Player::PollAttackInput() {
    if (IsKeyPressed(KEY_H)){
        _requested_state = uint8_t(PUNCH);
        _is_animating = true;
        return;
    } 
    if (IsKeyPressed(KEY_J)){
        _requested_state = uint8_t(KICK);
        _is_animating = true;
        return;
    }
    if (IsKeyPressed(KEY_K)){
        _requested_state = uint8_t(JUMP);
        _is_animating = true;
        return;
    }
    if (IsKeyPressed(KEY_L)){
        _requested_state = uint8_t(BLOCK);
        _is_animating = true;
        return;
    }
}

void Player::PollInput() {
    // MOVEMENT // 
    if (IsKeyDown(KEY_D)){
        _requested_state = uint8_t(MOVE_RIGHT);
        _is_animating = false;
        anim_frame_counter = 0;
        return;
    }   
    if (IsKeyDown(KEY_A)){
        _requested_state = uint8_t(MOVE_LEFT);
        _is_animating = false;
        anim_frame_counter = 0;
        return;
    } 
    if (IsKeyDown(KEY_W)){
        _requested_state = uint8_t(MOVE_UP);
        _is_animating = false;
        anim_frame_counter = 0;
        return;
    } 
    if (IsKeyDown(KEY_S)){
        _requested_state = uint8_t(MOVE_DOWN);
        _is_animating = false;
        anim_frame_counter = 0;
        return;
    }

    PollAttackInput();
    ProcessPlayerAnimLogic();

    if (IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && !_is_animating){
        _requested_state = uint8_t(IDLE);
        return;
    }

    if (IsKeyPressed(KEY_SPACE)){
        _ready = !_ready;
        return;
    }
    
}

bool Player::Ready() {
    return _ready;
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
