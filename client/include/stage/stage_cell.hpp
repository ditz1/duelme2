#pragma once

#include <globals.hpp>

typedef class StageCell {
public:
    StageCell();
    StageCell(Vector2 grid_location, char type, Rectangle rect, Color color);
    void Draw();
    Vector2 grid_location;
    bool collidable;
    char type;
    Rectangle rect;
    Color color;
    Texture2D tex;
} StageCell;
