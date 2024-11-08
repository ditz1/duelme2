#pragma once
#include <physics.hpp>

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