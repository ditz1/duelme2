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

static std::map<uint8_t, Rectangle> PlayerCells = {
    {0, Rectangle{0, 0, 0, 0}},
    {1, Rectangle{0, 0, 0, 0}},
    {2, Rectangle{0, 0, 0, 0}},
    {3, Rectangle{0, 0, 0, 0}}
};

typedef struct ServerStage {
    std::vector<Rectangle> cells;
    uint8_t cell_size;
} ServerStage;

