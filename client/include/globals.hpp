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
extern int num_players_connected;
extern std::vector<uint8_t> data_from_server;
extern int current_game_stage;
extern std::array<bool, 4> player_ready;
extern bool developer_mode;
extern int num_failed_pings;
extern bool in_loading_screen;
extern bool stage_sent;
extern bool stage_message_created;
extern float player_size;


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

enum Item {
    EMPTY,
    SHOTGUN,
    PISTOL
}

enum PlayerState {
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    PUNCH,
    KICK,
    JUMP,
    BLOCK,
    AIRBORNE,
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
    {PUNCH, "PUNCH"},
    {KICK, "KICK"},
    {JUMP, "JUMP"},
    {BLOCK, "BLOCK"},
    {AIRBORNE, "AIRBORNE"},
    {ALL_PLAYER_STATES, "ALL_PLAYER_STATES"},
    {UNREGISTERED, "UNREGISTERED"}
};

typedef struct GameState {
    std::array<uint8_t, 4> player_ids;
    std::array<uint8_t, 4> player_hps;
    std::array<uint8_t, 4> player_states;
    std::array<Vector2int, 4> player_positions;
    typedef union {
        uint16_t u;
        struct {
            uint8_t b0;
            uint8_t b1;
        } b;
    } u16tou8s;
    std::array<uint8_t, 28> ToBytes(){
        std::array<uint8_t, 28> bytes;
        for (int i = 0; i < 28; i+=7){
            bytes[i] = player_ids[i/7];
            bytes[i+1] = player_states[i/7];
            u16tou8s x;
            x.u = player_positions[i/7].x;
            bytes[i+2] = x.b.b1;
            bytes[i+3] = x.b.b0;
            u16tou8s y;
            y.u = player_positions[i/7].y;
            bytes[i+4] = y.b.b1;
            bytes[i+5] = y.b.b0;
            bytes[i+6] = player_hps[i/7];
        }
        return bytes;
    }
    void FromBytes(std::array<uint8_t, 32> bytes){
        for (int i = 1; i < 29; i+=7){
            player_ids[i/7] = bytes[i];
            player_states[i/7] = bytes[i+1];
            u16tou8s x;
            x.b.b1 = bytes[i+2];
            x.b.b0 = bytes[i+3];
            player_positions[i/7].x = x.u;
            u16tou8s y;
            y.b.b1 = bytes[i+4];
            y.b.b0 = bytes[i+5];
            player_positions[i/7].y = y.u;
            player_hps[i/7] = bytes[i+6];
        }
    }

} GameState;

std::string PlayerStateToString(PlayerState state); 
Vector2 V2intToV2(Vector2int v);
void HandleErrors(Connection& conn);
std::tuple<uint8_t, uint8_t> Float16ToBytes(float f);