#pragma once
#include <array>
#include <cstdint>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

#define msg_connect 0x1a
#define msg_disconnect 0x2a
#define msg_update 0x3a

#define msg_assign_id 0x4a
#define msg_end 0x5a // terminate char
#define msg_ping 0x6a

extern bool game_running;


typedef struct Vector2int {
    uint16_t x;
    uint16_t y;
} Vector2int;

// enum PlayerState {
//     MOVE_RIGHT, = 0x0
//     MOVE_LEFT, 0x1
//     MOVE_UP, 0x2
//     MOVE_DOWN, 0x3
//     IDLE, 0x4
//     ALL_PLAYER_STATES,
//     UNREGISTERED
// };


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

extern GameState game_state;

void InitGameState(GameState* game);
