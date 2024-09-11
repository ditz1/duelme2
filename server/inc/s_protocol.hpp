#pragma once
#include <array>
#include <cstdint>

#define msg_connect 0x1a
#define msg_disconnect 0x2a
#define msg_update 0x3a

#define msg_assign_id 0x4a
#define msg_end 0x5a // terminate char
#define msg_ping 0x6a

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
            uint8_t b1;
            uint8_t b0;
        } b;
    } u16tou8s;
    std::array<uint8_t, 24> ToBytes(){
        std::array<uint8_t, 24> bytes;
        for (int i = 0; i < int(bytes.size()); i+=6){
            bytes[i] = player_ids[i];
            bytes[i+1] = player_states[i];
            u16tou8s x;
            x.u = player_positions[i].x;
            bytes[i+2] = x.b.b1;
            bytes[i+3] = x.b.b0;
            u16tou8s y;
            y.u = player_positions[i].y;
            bytes[i+4] = y.b.b1;
            bytes[i+5] = y.b.b0;

        }
        return bytes;
    }
} GameState;