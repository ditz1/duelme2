#pragma once
#include <vector>
#include <physics.hpp>

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef class StageCell {
public:
    StageCell() {
        grid_pos = {0, 0};
        rect = {0, 0, 0, 0};
        collidable = false;
    }
    void Draw();
    Vector2 grid_pos;
    Rectangle rect;
    bool collidable;
private:
} StageCell;

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
    std::vector<GridCoords> stage_cells;
    std::vector<std::pair<int, int>> colls;
    std::vector<std::pair<int, StageCell>> colls_stage;
    int max_x;
    int max_y;
    int rows;
    int cols;
    int cell_size;
    int num_stage_cells;
} CollisionGrid;

void HandleCollisions(CollisionGrid& grid, GameState& game_state, std::vector<GridCoords>& search);
void GenerateCollisionGrid(CollisionGrid &collision_grid, int cell_size, int rows, int cols);
void ClearCollisionGrid(CollisionGrid &collision_grid);
void UpdateCollisionGrid(CollisionGrid &collision_grid, std::vector<Player> &players);
std::vector<GridCoords> GetCollisionSearch(CollisionGrid &collision_grid);
void DrawCollisionGrid(CollisionGrid &collision_grid);
void HandleCollisions(CollisionGrid &collision_grid, std::vector<Player> &players, std::vector<Vector2>& vels);