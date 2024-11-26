// spatial_grid.cpp
#include "spatial_grid.hpp"
#include "test_helpers.hpp"
#include <math.h>
#include <cstdio>


void SpatialGrid::Init(int rows, int cols, int cell_size) {
    this->rows = rows;
    this->cols = cols;
    this->cell_size = cell_size;
    this->num_cells = rows * cols;
    this->cells = std::vector<std::vector<Cell>>(num_cells);
}

bool SpatialGrid::IsValidCell(int i, int j) const {
    return i >= 0 && i < cols && j >= 0 && j < rows;
}

// In spatial_grid.cpp
std::vector<Vector2> SpatialGrid::GetOverlappingAndAdjacentCells(float grid_x, float grid_y, float width_cells, float height_cells) const {
    std::vector<Vector2> cells;
    
    // Calculate the grid cells the player actually occupies
    int start_i = floor(grid_x);
    int start_j = floor(grid_y);
    int end_i = floor(grid_x + width_cells);  // width in cells (1 for width = cell_size)
    int end_j = floor(grid_y + height_cells); // height in cells (2 for height = cell_size * 2)
    
    // Get occupied cells plus one cell padding
    for(int i = start_i - 1; i <= end_i + 1; i++) {
        for(int j = start_j - 1; j <= end_j + 1; j++) {
            if(IsValidCell(i, j)) {
                cells.push_back({(float)i, (float)j});
            }
        }
    }
    
    return cells;
}

void SpatialGrid::Insert(float grid_x, float grid_y, Player* player) {
    // Calculate cells covered by the rectangular player
    int start_i = floor(grid_x);
    int start_j = floor(grid_y);
    int end_i = floor(grid_x + (player->rect.width / cell_size));
    int end_j = floor(grid_y + (player->rect.height / cell_size));
    
    for(int i = start_i; i <= end_i; i++) {
        for(int j = start_j; j <= end_j; j++) {
            if(IsValidCell(i, j)) {
                uint64_t hash = HashCell(i, j);
                Cell cell = {i, j, cell_size, player};
                cells[hash].push_back(cell);
            }
        }
    }
}

// Add debug drawing implementation:
void SpatialGrid::DrawDebug() {
    // First draw grid lines
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < rows; j++) {
            DrawRectangleLines(
                i * cell_size,
                j * cell_size,
                cell_size,
                cell_size,
                GRAY
            );
        }
    }
    
    // Then draw occupied cells
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < rows; j++) {
            uint64_t hash = HashCell(i, j);
            if(!cells[hash].empty()) {
                // Draw occupied cells in semi-transparent red
                Color highlight = {230, 41, 55, 100};
                DrawRectangle(
                    i * cell_size,
                    j * cell_size,
                    cell_size,
                    cell_size,
                    highlight
                );
                
                // Draw number of objects in cell
                DrawText(TextFormat("%zu", cells[hash].size()), 
                    i * cell_size + cell_size/3, 
                    j * cell_size + cell_size/3, 
                    20, BLACK);
            }
        }
    }
}


uint64_t SpatialGrid::HashCell(int i, int j) {
    //printf("HashCell - i: %d, j: %d\n", i, j);
    uint64_t hash = (uint64_t)((i * 92837111LL) ^ (j * 689287499LL));
    //printf("raw hash | %llu\n", hash);
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