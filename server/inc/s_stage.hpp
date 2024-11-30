#pragma once
#include <physics.hpp>
#include <collision_grid.hpp>
#include <tuple>

typedef class ServerStage {
public:
    void LoadDataIntoCells();
    bool ProcessPlayerCollision(Vector2int player_position);
    CollisionIndex ProcessPlayerCollisionDirection(Vector2int player_position);
    CollisionGrid collision_grid;
    std::vector<Rectangle> cells;
    std::vector<uint8_t> data;
    uint8_t cell_size;
    float scale;
    int width;
    int height;
    int player_width;
    int player_height;
    int max_y_level;
    int max_x_level;
    int min_y_level;
    int min_x_level;
} ServerStage;