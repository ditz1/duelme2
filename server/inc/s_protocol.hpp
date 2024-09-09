#pragma once
#include <array>
#include <cstdint>

#define xCONNECT 0x1a
#define xDISCONNECT 0x2a
#define xUPDATE 0x3a
#define xASSIGN_ID 0x4a
#define xEND_MSG 0x5a
#define xPING 0x6a

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
    std::array<uint8_t, 16> ToBytes(){
        std::array<uint8_t, 16> bytes;
        for (int i = 0; i < int(bytes.size()); i+=4){
            bytes[i] = player_ids[i];
            bytes[i+1] = player_positions[i].x;
            bytes[i+2] = player_positions[i].y;
            bytes[i+3] = player_states[i];
        }
        return bytes;
    }
} GameState;