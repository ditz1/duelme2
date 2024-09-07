#pragma once
#include <emscripten/websocket.h>
#include <cstdio>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <map>
#include <protocol.hpp>

typedef struct Connection {
    int id;
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
    ALL_PLAYER_STATES,
    UNREGISTERED
};

static std::map<PlayerState, std::string> PlayerStateToStringMap = {
    {IDLE, "IDLE"},
    {MOVE_RIGHT, "MOVE_RIGHT"},
    {MOVE_LEFT, "MOVE_LEFT"},
    {MOVE_UP, "MOVE_UP"},
    {MOVE_DOWN, "MOVE_DOWN"},
};

typedef struct GameState {
    std::vector<int> player_ids;
    std::vector<Vector2> player_positions;
    std::vector<int> player_states;
    
    std::string ToBytes(){
        std::string bytes;
        for (auto& player_id : player_ids) {
            bytes += std::to_string(player_id);
        }
        for (auto& player_position : player_positions) {
            bytes += std::to_string(player_position.x);
            bytes += std::to_string(player_position.y);
        }
        for (auto& player_state : player_states) {
            bytes += std::to_string(player_state);
        }
        return bytes;
    }
} GameState;

std::string PlayerStateToString(PlayerState state); 