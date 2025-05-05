#include <stage_cell.hpp>

StageCell::StageCell() {
    grid_location = {0, 0};
    type = ' ';
    rect = {0, 0, 0, 0};
    collidable = false;
    color = WHITE;
    tex = {0};
}

StageCell::StageCell(Vector2 grid_location, char type, Rectangle rect, Color color) {
    this->grid_location = grid_location;
    this->type = type;
    this->rect = rect;
    this->color = color;
}

void StageCell::Draw() {
    DrawTexture(tex, rect.x, rect.y, WHITE);
    //DrawRectangleRec(rect, color);
}