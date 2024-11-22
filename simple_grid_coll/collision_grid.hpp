#pragma once
#include <vector>
#include <stage_cell.hpp>

typedef struct GridCoords {
    int x;
    int y;
} GridCoords;

typedef struct Cell {
    GridCoords idx;
    bool is_occupied;
} Cell;

typedef struct CollisionGrid {
    std::vector<StageCell> stage;
    std::vector<std::vector<Cell>> cells;
    std::vector<GridCoords> occupied_cells;
    int max_x;
    int max_y;
    int rows;
    int cols;
    int cell_size;
} CollisionGrid;

void GenerateCollisionGrid(CollisionGrid &collision_grid, int cell_size, int rows, int cols);
void ClearCollisionGrid(CollisionGrid &collision_grid);
void UpdateCollisionGrid(CollisionGrid &collision_grid, std::vector<Rectangle> &players);
std::vector<GridCoords> GetCollisionSearch(CollisionGrid &collision_grid);
void DrawCollisionGrid(CollisionGrid &collision_grid);