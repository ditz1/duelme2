#pragma once

#include <globals.hpp>
#include <networking.hpp>

class Player {
public:
    Player();
    ~Player();
    PlayerState State();
    PlayerState RequestedState();
    void PollInput();
    void Update(GameState& game);
    void Draw();
    uint8_t Id();
    void SetId(int id);
    Vector2int Position();
    void inline SetPosition(Vector2int position) { _position = position; }
    void inline SetState(PlayerState state) { _state = state; }
    int inline Hp() { return _hp; }
    void inline SetHp(int hp) { _hp = hp; }
private:
    uint8_t _id;
    Vector2int _position;
    int _hp;
    PlayerState _state;
    uint8_t _requested_state;
    Color _color;
};