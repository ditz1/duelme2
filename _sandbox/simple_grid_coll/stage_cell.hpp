#pragma once

#include <array>
#include <raylib.h>
#include <vector>


typedef class StageCell {
public:
    StageCell();
    void Draw();
    Vector2 grid_pos;
    Rectangle rect;
    bool collidable;
    Color color;

} StageCell;