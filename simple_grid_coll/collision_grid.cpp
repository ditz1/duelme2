#include "collision_grid.hpp"
#include <iostream>
#include <set>

void GenerateCollisionGrid(CollisionGrid &collision_grid, int cell_size, int rows, int cols){
    std::cout << "generating" << std::endl;
    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    std::cout << "cell_size: " << cell_size << std::endl;

    collision_grid.cell_size = cell_size;
    collision_grid.rows = rows;    
    collision_grid.cols = cols;

    collision_grid.cells.resize(rows);
    for (int i = 0; i < rows; i++) {
        collision_grid.cells[i].resize(cols);
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            collision_grid.cells[i][j].idx.x = j;
            collision_grid.cells[i][j].idx.y = i;
            collision_grid.cells[i][j].is_occupied = false;
        }
    }
    std::cout << collision_grid.cells.size() << std::endl;
    for (auto& i : collision_grid.stage){
        std::cout << i.grid_pos.x << " , " << i.grid_pos.y << std::endl;
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            if (collision_grid.max_x < j * cell_size) {
                collision_grid.max_x = j * cell_size;
            }
            if (collision_grid.max_y < i * cell_size) {
                collision_grid.max_y = i * cell_size;
            }
        }
    }
}


void DrawCollisionGrid(CollisionGrid& grid){
    int h = 0;
    for (auto& i : grid.occupied_cells){
        DrawRectangle(i.x * grid.cell_size, i.y * grid.cell_size, grid.cell_size, grid.cell_size, PURPLE);
        DrawText(TextFormat("%d, %d", i.x, i.y), i.x * grid.cell_size + 5, i.y * grid.cell_size + 4, 24, RAYWHITE);
        h++;   
    }
}


void UpdateCollisionGrid(CollisionGrid &grid, std::vector<Rectangle> &players){
    grid.occupied_cells.clear();
    for (auto& i : players){
        int x = floor((i.x + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        int y = floor((i.y + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (i.x >= grid.max_x) x = grid.cols;
        if (i.y >= grid.max_y) y = grid.rows;
        // x, y is top left cell adjacent of player
        grid.cells[x][y].is_occupied = true;
        grid.occupied_cells.push_back({x, y});
    }
}

std::vector<GridCoords> GetCollisionSearch(CollisionGrid& grid) {
    std::vector<GridCoords> search;
    std::set<std::pair<int, int>> uniqueCells; // To prevent duplicates
    int numX = grid.max_x / grid.cell_size;
    int numY = grid.max_y / grid.cell_size;
    
    for (auto& i : grid.occupied_cells) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                int newX = i.x + j;
                int newY = i.y + k;
                if (newX >= 0 && newX < grid.rows && newY >= 0 && newY < grid.cols) {
                    // Only add if not already in set
                    if (uniqueCells.insert({newX, newY}).second) {
                        search.push_back({newX, newY});
                    }
                }
            }
        }
    }
    return search;
}

