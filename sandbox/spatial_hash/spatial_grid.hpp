// spatial_grid.hpp
#pragma once
#include <vector>
#include <cstdint>
#include "raylib.h"

class Player;  // Forward declaration

struct Cell {
    int i, j;
    int cell_size;
    Player* player;
};

class SpatialGrid {
public:
    int Rows() const { return rows; }
    int Cols() const { return cols; }
    void Init(int rows, int cols, int cell_size);
    void DrawDebug();
    uint64_t HashCell(int i, int j);
    void Insert(float x, float y, Player* player);
    void Clear();
    const std::vector<Cell>& GetCellsAt(uint64_t hash) const;
    std::vector<Vector2> GetOverlappingAndAdjacentCells(float x, float y, float width, float height) const;
    bool IsValidCell(int i, int j) const;

private:
    int rows;
    int cols;
    int cell_size;
    int num_cells;
    std::vector<std::vector<Cell>> cells;

};

