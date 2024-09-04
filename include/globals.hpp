#pragma once
#include <emscripten/websocket.h>
#include <cstdio>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <map>

typedef struct Connection {
    EMSCRIPTEN_WEBSOCKET_T ws;
    EmscriptenWebSocketMessageEvent* last_received;
    bool connected;
} Connection;

enum PlayerState {
    IDLE,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,    
};

static std::map<PlayerState, std::string> PlayerStateToString = {
    {IDLE, "IDLE"},
    {MOVE_RIGHT, "MOVE_RIGHT"},
    {MOVE_LEFT, "MOVE_LEFT"},
    {MOVE_UP, "MOVE_UP"},
    {MOVE_DOWN, "MOVE_DOWN"},
};

struct GameState {
    std::vector<int> player_ids;
    std::vector<Vector2> player_positions;
    std::vector<int> player_states;
};