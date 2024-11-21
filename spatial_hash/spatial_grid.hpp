// spatial_grid.hpp
#pragma once
#include <vector>
#include <cstdint>

class Player;  // Forward declaration

struct Cell {
    int i, j;
    int cell_size;
    Player* player;
};

class SpatialGrid {
private:
    int rows;
    int cols;
    int cell_size;
    int num_cells;
    std::vector<std::vector<Cell>> cells;

public:
    int Rows() const { return rows; }
    int Cols() const { return cols; }
    void Init(int rows, int cols, int cell_size);
    uint64_t HashCell(int i, int j);
    void Insert(float x, float y, Player* player);
    void Clear();
    const std::vector<Cell>& GetCellsAt(uint64_t hash) const;
};