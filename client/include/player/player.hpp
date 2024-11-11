#pragma once

#include <globals.hpp>
#include <networking.hpp>

typedef struct AnimData {
    Image img;
    Texture2D tex;
    int fc = 0;
} AnimData;

typedef struct DrawData {
    Rectangle source;
    Rectangle dest;
    float scale = 5.0f;
} DrawData;

typedef class Player {
public:
    Player();
    ~Player();
    PlayerState State();
    PlayerState RequestedState();
    void inline SetRequestedState(PlayerState state) { _requested_state = uint8_t(state); }
    void inline SetIsAnimating(bool is_animating) { _is_animating = is_animating; }
    void PollInput();
    void Update();
    void Draw();
    bool inline IsAnimating() { return _is_animating; }
    bool inline IsAttacking() { return _is_attacking; }
    void inline SetAttacking(bool is_attacking) { _is_attacking = is_attacking; }   
    uint8_t Id();
    void SetId(int id);
    bool Ready();
    int inline FaceDir() { return _player_face_dir; }
    Vector2int Position();
    void inline SetPosition(Vector2int position) { _position = position; }
    void inline SetState(PlayerState state) { _state = state; }
    int inline Hp() { return _hp; }
    Rectangle inline Bounds() { return _bounds; }
    void inline SetHp(int hp) { _hp = hp; }
    void ProcessPlayerAnimLogic();    
    void SetTexture(int texture_id);
    void LoadTextures();
    void PlayAnimOnce(PlayerState state);
    void AssignTexture(PlayerState state);
    void SetFaceDir(int dir);
    
    DrawData draw_data;
    PlayerState last_state;
    // anim stuff, supposed to replicate "dummy" from anim_testing
    int fc; // frame counter total, mainly needed for delay
    int anim_current_frame;
    unsigned int buffer_offset; // offset to move pointer in img data
    int current_anim;
    int fc_delay; // this should be global
    Texture2D* tex; // this should be loaded from img anyway
    Image* img;
    std::array<AnimData, 7> texs;
    int face_dir;
    Item item;

private:
    void PollAttackInput();
    void ResetState();
    bool _textures_loaded;
    uint8_t _id;
    Vector2int _position;
    int _hp;
    bool _is_animating;
    int _player_face_dir;
    bool _ready;
    PlayerState _state;
    uint8_t _requested_state;
    Color _color;
    bool _is_attacking;
    Rectangle _bounds; // this will be used for collision detection
    // image stuff, these will be assigned from characters later, for now just a placeholder

} Player;