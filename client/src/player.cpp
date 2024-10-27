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
    fc = 0;
    anim_frame_counter = 0;
    anim_current_frame = 0;
    buffer_offset = 0;
    fc_delay = 12;
}

Player::~Player() {
    // for some reason unloading here causes a segfault ????
    //UnloadTexture(tex);
    //UnloadImage(img);
}

Vector2int Player::Position() {
    return _position;
}

void Player::SetTexture(int texture_id) { // later we will have a map of textures
    if (texture_id < 0) return;
    std::cout << "Loading texture" << std::endl;
    img = LoadImageAnim("/assets/stickman.gif", &anim_frame_counter);
    tex = LoadTextureFromImage(img);
}


void Player::ProcessPlayerAnimLogic() {

    if (_requested_state < 4 || _requested_state == PlayerState::IDLE) {
        _is_animating = false;
        anim_frame_counter = 0;
    }

    if (anim_frame_counter >= 60) {
        anim_frame_counter = 0;
        anim_current_frame = 0;
        return;
    }

     // this is 4 because single byte per channel (RGBA)
    buffer_offset = img.width * img.height * 4 * anim_current_frame;
    
    if (_is_animating) {
        anim_frame_counter++;
        fc++; // total fc
        if (fc >= fc_delay) {
            // move to next frame
            anim_current_frame++;
            if (anim_current_frame >= anim_frame_counter) anim_current_frame = 0; // if final frame is reached we return to first frame
            // get memory offset position for next frame data in image.data
            // this is 4 because single byte per channel (RGBA)
            buffer_offset = img.width*img.height*4*anim_current_frame;
            // WARNING: data size (frame size) and pixel format must match already created texture
            // "void* pixels" is pointer to image raw data
            UpdateTexture(tex, ((unsigned char *)img.data) + buffer_offset);
            fc = 0;
        }
    }

    if (_requested_state <= 7 && _requested_state >= 4) {
        _is_animating = true;
            if (anim_frame_counter >= 60){
                anim_frame_counter = 0;
                anim_current_frame = 0;
                _is_animating = false;
        }
    }


    if (_id != this_client_id) return;

    if (PlayerState(_requested_state) != _state) {
        anim_frame_counter = 0;
    }

}

void Player::PollAttackInput() {
    if (IsKeyPressed(KEY_H)){
        _requested_state = uint8_t(PUNCH);
        return;
    } 
    if (IsKeyPressed(KEY_J)){
        _requested_state = uint8_t(KICK);
        return;
    }
    if (IsKeyPressed(KEY_K)){
        _requested_state = uint8_t(JUMP);
        return;
    }
    if (IsKeyPressed(KEY_L)){
        _requested_state = uint8_t(BLOCK);
        return;
    }
}

void Player::PollInput() {
    // MOVEMENT // 
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

    PollAttackInput();
    ProcessPlayerAnimLogic();

    if (IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && !_is_animating){
        _requested_state = uint8_t(IDLE);
        anim_frame_counter = 0;
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
