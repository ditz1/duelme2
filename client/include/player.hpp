#pragma once

#include <globals.hpp>
#include <networking.hpp>


class Player {
public:
    Player();
    ~Player();
    PlayerState State();
    PlayerState RequestedState();
    void inline SetRequestedState(PlayerState state) { _requested_state = uint8_t(state); }
    void PollInput();
    void Update(GameState& game);
    void Draw();
    uint8_t Id();
    void SetId(int id);
    bool Ready();
    int inline FaceDir() { return _player_face_dir; }
    Vector2int Position();
    void inline SetPosition(Vector2int position) { _position = position; }
    void inline SetState(PlayerState state) { _state = state; }
    int inline Hp() { return _hp; }
    void inline SetHp(int hp) { _hp = hp; }
    int anim_frame_counter;

private:
    void PollAttackInput();
    uint8_t _id;
    Vector2int _position;
    int _hp;
    bool _is_animating;
    int _current_anim;
    int _player_face_dir;
    bool _ready;
    PlayerState _state;
    uint8_t _requested_state;
    Color _color;
};