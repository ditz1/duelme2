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
    int Id();
    void inline SetId(int id) { _id = id; }
    Vector2 Position();
private:
    int _id;
    Vector2 _position;
    PlayerState _state;
    int8_t _requested_state;
    Color color;
};