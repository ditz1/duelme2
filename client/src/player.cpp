#include <player.hpp>
#include <player_anims.hpp>

Player::Player() {
    _requested_state = 0;
    _state = IDLE;
    _position = {0xEEEE, 0xDDDD};
    current_anim = 0;
    _is_animating = false;
    _player_face_dir = 1;
    _hp = 100;
    fc = 0;
    anim_current_frame = 0;
    buffer_offset = 0;
    fc_delay = 12;
    _textures_loaded = false;
}

Player::~Player() {
    // for some reason unloading here causes a segfault ????
    //UnloadTexture(tex);
    //UnloadImage(img);
}

Vector2int Player::Position() {
    return _position;
}

void Player::LoadTextures() {
    if (_textures_loaded) return;
    std::string prefix = "red";
    switch (_id) {
        case 0:
            prefix = "red";
            break;
        case 1:
            prefix = "blue";
            break;
        case 2:
            prefix = "green";
            break;
        case 3:
            prefix = "yellow";
            break;
    }

    texs[0].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_idle.gif").c_str(), &texs[0].fc);
    texs[0].tex = LoadTextureFromImage(texs[0].img);

    texs[1].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_walk.gif").c_str(), &texs[1].fc);
    texs[1].tex = LoadTextureFromImage(texs[1].img);

    texs[2].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_punch.gif").c_str(), &texs[2].fc);
    texs[2].tex = LoadTextureFromImage(texs[2].img);

    texs[3].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_kick.gif").c_str(), &texs[3].fc);
    texs[3].tex = LoadTextureFromImage(texs[3].img);

    texs[4].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_jump.gif").c_str(), &texs[4].fc);
    texs[4].tex = LoadTextureFromImage(texs[4].img);

    texs[5].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_enterblock.gif").c_str(), &texs[5].fc);
    texs[5].tex = LoadTextureFromImage(texs[5].img);

    texs[6].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_die.gif").c_str(), &texs[6].fc);
    texs[6].tex = LoadTextureFromImage(texs[6].img);

    img = &texs[0].img;
    tex = &texs[0].tex;

    _textures_loaded = true;

}
void Player::SetTexture(int texture_id) { // later we will have a map of textures
    if (texture_id < 0) return;
    std::cout << "Loading texture" << std::endl;
    //img = LoadImageAnim("/assets/test.gif", &anim_frame_counter);
    //tex = LoadTextureFromImage(img);
}

void Player::AssignTexture(PlayerState state){
    _is_animating = true;
    
    switch (state) {
        case IDLE:
            img = &texs[0].img;
            tex = &texs[0].tex;
            current_anim = 0;
            break;
        case MOVE_RIGHT:
        case MOVE_LEFT: 
            img = &texs[1].img;
            tex = &texs[1].tex;
            current_anim = 1;
            break;
        case PUNCH:
            img = &texs[2].img;
            tex = &texs[2].tex;
            current_anim = 2;
            break;
        case KICK:
            img = &texs[3].img;
            tex = &texs[3].tex;
            current_anim = 3;
            break;
        case MOVE_UP:
            img = &texs[4].img;
            tex = &texs[4].tex;
            current_anim = 4;
            break;
        case BLOCK:
            img = &texs[5].img;
            tex = &texs[5].tex;
            current_anim = 5;
            break;
        case MOVE_DOWN:
            img = &texs[6].img;
            tex = &texs[6].tex;
            current_anim = 6;
            break;
        default:
            break;
    }
}


void Player::ProcessPlayerAnimLogic() {

    if (anim_current_frame >= 60) {
        texs[current_anim].fc = 0;
        anim_current_frame = 0;
        return;
    }

     // this is 4 because single byte per channel (RGBA)
    buffer_offset = img->width * img->height * 4 * anim_current_frame;

    fc++; // total fc
    if (fc >= fc_delay) {
        // move to next frame
        anim_current_frame++;
        if (anim_current_frame >= texs[current_anim].fc) anim_current_frame = 0; // if final frame is reached we return to first frame
        // get memory offset position for next frame data in image.data
        // this is 4 because single byte per channel (RGBA)
        buffer_offset = img->width*img->height*4*anim_current_frame;
        // WARNING: data size (frame size) and pixel format must match already created texture
        // "void* pixels" is pointer to image raw data
        UpdateTexture(*tex, ((unsigned char *)img->data) + buffer_offset);
        fc = 0;
    }

    if (_id != this_client_id) return;

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

    if (IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && IsKeyUp(KEY_H) && IsKeyUp(KEY_J) && IsKeyUp(KEY_K) && IsKeyUp(KEY_L)){
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
    LoadTextures();
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
