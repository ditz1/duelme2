// spatial_grid.cpp
#include "spatial_grid.hpp"
#include <math.h>
#include <cstdio>


void SpatialGrid::Init(int rows, int cols, int cell_size) {
    this->rows = rows;
    this->cols = cols;
    this->cell_size = cell_size;
    this->num_cells = rows * cols;
    this->cells = std::vector<std::vector<Cell>>(num_cells);
}

void SpatialGrid::Insert(float x, float y, Player* player) {
    int i = floor(x);  // Remove the cell_size division here since we're passing pre-divided coords
    int j = floor(y);
    printf("Insert - x: %f, y: %f, i: %d, j: %d\n", x, y, i, j);
    uint64_t hash = HashCell(i, j);
    printf("hash: %ld, num_cells: %d\n", hash, num_cells);

    Cell cell = {i, j, cell_size, player};
    cells[hash].push_back(cell);
}

uint64_t SpatialGrid::HashCell(int i, int j) {
    printf("HashCell - i: %d, j: %d\n", i, j);
    uint64_t hash = (uint64_t)((i * 92837111LL) ^ (j * 689287499LL));
    printf("raw hash | %llu\n", hash);
    return hash % num_cells;  
}

void SpatialGrid::Clear() {
    for (auto& cell_list : cells) {
        cell_list.clear();
    }
}

const std::vector<Cell>& SpatialGrid::GetCellsAt(uint64_t hash) const {
    return cells[hash];
}