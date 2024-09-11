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
        for (int i = 1; i < 26; i+=6){
            this->player_ids[i/6] = bytes[i];
            this->player_states[i/6] = bytes[i+1];
            u16tou8s x;
            x.b.b1 = bytes[i+2];
            x.b.b0 = bytes[i+3];
            this->player_positions[i/6].x = x.u;
            u16tou8s y;
            y.b.b1 = bytes[i+4];
            y.b.b0 = bytes[i+5];
            this->player_positions[i/6].y = y.u;
        }
        printf("FROM BYTES\n");
        for (int i = 0; i < 26; i++) {
            printf("%x | ", bytes[i]);
        }
        printf("\n");
    }

} GameState;

extern GameState game_state;

void InitGameState(GameState* game);
