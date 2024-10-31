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
    fc_delay = 6;
    _textures_loaded = false;
    _is_attacking = false;
    _ready = false;
    _name = "";
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
    std::array<std::string, 7> anims = {"idle", "walk", "punch", "kick", "jump", "enterblock", "die"};
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

    for (int i = 0; i < 7; i++) {
        texs[i].img = LoadImageAnim(("assets/" + prefix + "/" + prefix + "_" + anims[i] + ".gif").c_str(), &texs[i].fc);
        texs[i].tex = LoadTextureFromImage(texs[i].img);
    }
    draw_data.source = {0.0f, 0.0f, (float)texs[0].img.width, (float)texs[0].img.height};
    draw_data.dest = {0.0f, 0.0f, (float)texs[0].img.width * draw_data.scale, (float)texs[0].img.height * draw_data.scale};


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

void Player::SetFaceDir(int dir) {
    _player_face_dir = dir;
}

void Player::ProcessPlayerAnimLogic() {

    if (_state == MOVE_LEFT && _player_face_dir == 1) {
        _player_face_dir = -1;
    } else if (_state == MOVE_RIGHT && _player_face_dir == -1) {
        _player_face_dir = 1;
    }

    if (_is_attacking && (anim_current_frame >= texs[current_anim].fc - 1)) {
        _is_attacking = false;
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
    if (IsKeyPressed(KEY_H) && !_is_attacking){
        _requested_state = uint8_t(PUNCH);
        _is_attacking = true;
        anim_current_frame = 0;
        return;
    } 
    if (IsKeyPressed(KEY_J) && !_is_attacking){
        _requested_state = uint8_t(KICK);
        _is_attacking = true;
        anim_current_frame = 0;
        return;
    }
    if (IsKeyPressed(KEY_K) && !_is_attacking){
        _requested_state = uint8_t(MOVE_UP);
        _is_attacking = true;
        anim_current_frame = 0;
        return;
    }
    if (IsKeyPressed(KEY_L) && !_is_attacking){
        _requested_state = uint8_t(BLOCK);
        _is_attacking = true;
        anim_current_frame = 0;
        return;
    }
}

void Player::ResetState() {
}

void Player::PollInput() {

    if (IsKeyPressed(KEY_SPACE)){
        _ready = !_ready;
        return;
    }
    // MOVEMENT // 
    ProcessPlayerAnimLogic();

    if (IsKeyDown(KEY_D) && !_is_attacking){
        _requested_state = uint8_t(MOVE_RIGHT);
        return;
    }   
    if (IsKeyDown(KEY_A) && !_is_attacking){
        _requested_state = uint8_t(MOVE_LEFT);
        return;
    } 
    if (IsKeyDown(KEY_W) && !_is_attacking){
        _requested_state = uint8_t(MOVE_UP);
        return;
    } 
    if (IsKeyDown(KEY_S) && !_is_attacking){
        _requested_state = uint8_t(MOVE_DOWN);
        return;
    }

    PollAttackInput();

    if (IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S) 
        && IsKeyUp(KEY_H) && IsKeyUp(KEY_J) && IsKeyUp(KEY_K) && IsKeyUp(KEY_L)
        && !_is_attacking){
        _requested_state = uint8_t(IDLE);
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
            //_name == "" ? _name = "P1" : _name;
            break;
        case 1:
            _color = BLUE;
            //_name == "" ? _name = "P2" : _name;
            break;
        case 2:
            _color = GREEN;
            //_name == "" ? _name = "P3" : _name;
            break;
        case 3:
            _color = YELLOW;
            //_name == "" ? _name = "P4" : _name;
            break;
    }
    LoadTextures();
}

PlayerState Player::RequestedState() {
    return PlayerState(_requested_state);
}


void Player::Draw() {
    _bounds.x = V2intToV2(_position).x - tex->width;
    _bounds.y = V2intToV2(_position).y - (tex->height) - 20;
    _bounds.width = tex->width * 2;
    _bounds.height = tex->height * 3;

    Vector2 pos = V2intToV2(_position);
    float scale = 3.0f;
    DrawCircleV(V2intToV2(_position), 20, _color);
    pos.x -= ((tex->width / 2) * draw_data.scale);
    pos.y -= ((tex->height / 2) * draw_data.scale);
    
    draw_data.source.width *= _player_face_dir;
    pos.x *= -1.0f;
    pos.y *= -1.0f;
    DrawTexturePro(*tex, draw_data.source, draw_data.dest, pos, 0.0f, RAYWHITE);
    DrawRectangleLinesEx(_bounds, 2, _color);
}

uint8_t Player::Id() {
    return _id;
}
