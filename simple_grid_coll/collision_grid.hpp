#pragma once
#include <vector>
#include <stage_cell.hpp>


typedef struct Player {
    Rectangle rect1; //lower
    Rectangle rect2; //upper
    int id;
} Player;

typedef struct GridCoords {
    int x;
    int y;
    int pid;
} GridCoords;

typedef struct Cell {
    GridCoords idx;
    bool is_occupied;
} Cell;

typedef struct CollisionGrid {
    std::vector<StageCell> stage;
    std::vector<std::vector<Cell>> cells;
    std::vector<GridCoords> occupied_cells;
    std::vector<std::pair<int, int>> colls;
    int max_x;
    int max_y;
    int rows;
    int cols;
    int cell_size;
} CollisionGrid;

void GenerateCollisionGrid(CollisionGrid &collision_grid, int cell_size, int rows, int cols);
void ClearCollisionGrid(CollisionGrid &collision_grid);
void UpdateCollisionGrid(CollisionGrid &collision_grid, std::vector<Player> &players);
std::vector<GridCoords> GetCollisionSearch(CollisionGrid &collision_grid);
void DrawCollisionGrid(CollisionGrid &collision_grid);
void HandleCollisions(CollisionGrid &collision_grid, std::vector<Player> &players, std::vector<Vector2>& vels);