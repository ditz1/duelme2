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
            uint8_t b0;
            uint8_t b1;
        } b;
    } u16tou8s;
    std::array<uint8_t, 24> ToBytes(){
        std::array<uint8_t, 24> bytes;
        for (int i = 0; i < 24; i+=6){
            bytes[i] = player_ids[i/6];
            bytes[i+1] = player_states[i/6];
            u16tou8s x;
            x.u = player_positions[i/6].x;
            bytes[i+2] = x.b.b1;
            bytes[i+3] = x.b.b0;
            u16tou8s y;
            y.u = player_positions[i/6].y;
            bytes[i+4] = y.b.b1;
            bytes[i+5] = y.b.b0;
        }
        printf("TO BYTES\n");
        for (int i = 0; i < 24; i++) {
            printf("%x | ", bytes[i]);
        }
        printf("\n");
        return bytes;
    }
    void FromBytes(std::array<uint8_t, 26> bytes){
        for (int i = 1; i < 25; i+=6){
            player_ids[i/6] = bytes[i];
            player_states[i/6] = bytes[i+1];
            u16tou8s x;
            x.b.b1 = bytes[i+2];
            x.b.b0 = bytes[i+3];
            player_positions[i/6].x = x.u;
            u16tou8s y;
            y.b.b1 = bytes[i+4];
            y.b.b0 = bytes[i+5];
            player_positions[i/6].y = y.u;
        }
        printf("FROM BYTES\n");
        for (int i = 0; i < 4; i++){
            printf("player: %x\n", player_ids[i]);
            printf("state: %x\n", player_states[i]);
            printf("pos: %x , %x\n", player_positions[i].x, player_positions[i].y);
        }
        printf("\n");
    }

} GameState;

std::string PlayerStateToString(PlayerState state); 
Vector2 V2intToV2(Vector2int v);
