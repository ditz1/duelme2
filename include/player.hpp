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
    void inline SetId(int id) { _id = uint8_t(id); }
    Vector2int Position();
private:
    uint8_t _id;
    Vector2int _position;
    PlayerState _state;
    uint8_t _requested_state;
    Color color;
};