#pragma once
#include <s_protocol.hpp>
#include <cstdint>
#include <map>

typedef struct Rectangle {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} Rectangle;

typedef union U16ToU8 {
    uint16_t u;
    struct {
        uint8_t b0;
        uint8_t b1;
    } b;
} U16ToU8;

typedef class ServerStage {
public:
    void LoadDataIntoCells();
    bool ProcessPlayerCollision(Vector2int player_position);
    std::vector<Rectangle> cells;
    std::vector<uint8_t> data;
    uint8_t cell_size;
    float scale;
    int width;
    int height;
    int player_width;
    int player_height;
} ServerStage;

