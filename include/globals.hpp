#pragma once
#include <emscripten/websocket.h>
#include <cstdio>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <protocol.hpp>
#include <cstdint>

extern int this_client_id;

typedef struct Vector2int {
    uint16_t x;
    uint16_t y;
} Vector2int;


typedef struct Connection {
    int id;
    EMSCRIPTEN_WEBSOCKET_T ws;
    EmscriptenWebSocketMessageEvent* last_received;
    bool connected;
} Connection;

enum PlayerState {
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    IDLE,
    ALL_PLAYER_STATES,
    UNREGISTERED
};

static std::map<PlayerState, std::string> PlayerStateToStringMap = {
    {IDLE, "IDLE"},
    {MOVE_RIGHT, "MOVE_RIGHT"},
    {MOVE_LEFT, "MOVE_LEFT"},
    {MOVE_UP, "MOVE_UP"},
    {MOVE_DOWN, "MOVE_DOWN"},
    {ALL_PLAYER_STATES, "ALL_PLAYER_STATES"},
    {UNREGISTERED, "UNREGISTERED"},
};

typedef struct GameState {
    std::array<uint8_t, 4> player_ids;
    std::array<Vector2int, 4> player_positions;
    std::array<uint8_t, 4> player_states;
    typedef union {
        uint16_t u;
        struct {
            uint8_t b1;
            uint8_t b0;
        } b;
    } u16tou8s;
    std::array<uint8_t, 16> ToBytes(){
        std::array<uint8_t, 16> bytes;
        for (int i = 0; i < bytes.size(); i+=4){
            bytes[i] = player_ids[i];
            bytes[i+1] = player_positions[i].x;
            bytes[i+2] = player_positions[i].y;
            bytes[i+3] = player_states[i];
        }
        return bytes;
    }
} GameState;

std::string PlayerStateToString(PlayerState state); 
Vector2 V2intToV2(Vector2int v);
