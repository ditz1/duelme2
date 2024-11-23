#include "collision_grid.hpp"
#include <iostream>
#include <set>

void GenerateCollisionGrid(CollisionGrid &grid, int cell_size, int rows, int cols){
    std::cout << "generating" << std::endl;
    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    std::cout << "cell_size: " << cell_size << std::endl;

    grid.cell_size = cell_size;
    grid.rows = rows;    
    grid.cols = cols;

    grid.cells.resize(rows);
    for (int i = 0; i < rows; i++) {
        grid.cells[i].resize(cols);
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            grid.cells[i][j].idx.x = j;
            grid.cells[i][j].idx.y = i;
            grid.cells[i][j].is_occupied = false;
        }
    }
    std::cout << grid.cells.size() << std::endl;
    for (auto& i : grid.stage){
        std::cout << i.grid_pos.x << " , " << i.grid_pos.y << std::endl;
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            if (grid.max_x < j * cell_size) {
                grid.max_x = j * cell_size;
            }
            if (grid.max_y < i * cell_size) {
                grid.max_y = i * cell_size;
            }
        }
    }

    for (auto& i : grid.stage){
        int x = i.grid_pos.x;
        int y = i.grid_pos.y;
        if (i.collidable){
            grid.cells[x][y].is_occupied = true;
            grid.stage_cells.push_back({x, y, -1});
        }
    }   
    //grid.num_stage_cells = grid.stage_cells.size();
    //grid.occupied_cells.reserve(grid.num_stage_cells);
    //std::copy(grid.stage_cells.begin(), grid.stage_cells.end(), std::back_inserter(grid.occupied_cells));
}


void DrawCollisionGrid(CollisionGrid& grid){
    int h = 0;
    for (auto& i : grid.occupied_cells){
        DrawRectangle(i.x * grid.cell_size, i.y * grid.cell_size, grid.cell_size, grid.cell_size, PURPLE);
        DrawText(TextFormat("%d, %d", i.x, i.y), i.x * grid.cell_size + 5, i.y * grid.cell_size + 4, 24, RAYWHITE);
        h++;   
    }
}


void UpdateCollisionGrid(CollisionGrid &grid, std::vector<Player> &players){
    grid.occupied_cells.clear();

    for (auto& i : players){
        int x1 = floor((i.rect1.x + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        int y1 = floor((i.rect1.y + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (i.rect1.x >= grid.max_x) x1 = grid.cols;
        if (i.rect1.y >= grid.max_y) y1 = grid.rows;
        // x, y is top left cell adjacent of player
        grid.cells[x1][y1].is_occupied = true;
        grid.occupied_cells.push_back({x1, y1, i.id});

        int x2 = floor((i.rect2.x + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        int y2 = floor((i.rect2.y + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        if (x2 < 0) x2 = 0;
        if (y2 < 0) y2 = 0;
        if (i.rect2.x >= grid.max_x) x2 = grid.cols;
        if (i.rect2.y >= grid.max_y) y2 = grid.rows;
        grid.cells[x2][y2].is_occupied = true;
        grid.occupied_cells.push_back({x2, y2, i.id});
    }
}

std::vector<GridCoords> GetCollisionSearch(CollisionGrid& grid) {
    grid.colls.clear();
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
                    // Check for player collisions
                    for (auto& other : grid.occupied_cells) {
                        if (other.pid != i.pid && // Different players
                            other.x == newX && other.y == newY) { // Same or adjacent cell
                            if (grid.cells[newX][newY].is_occupied) {
                                grid.colls.push_back({i.pid, other.pid});
                            }
                        }
                    }

                    for (auto& other : grid.stage_cells) {
                        if (other.x == newX && other.y == newY) {
                            if (other.pid < 0) {
                                StageCell sc;
                                sc.grid_pos = {(float)newX, (float)newY};
                                sc.rect.x = newX * grid.cell_size;
                                sc.rect.y = newY * grid.cell_size;
                                sc.rect.width = grid.cell_size;
                                sc.rect.height = grid.cell_size;
                                grid.colls_stage.push_back({i.pid, sc});
                            }
                        }
                    }

                    // Only add if not already in set
                    if (uniqueCells.insert({newX, newY}).second) {
                        search.push_back({newX, newY, i.pid});
                    }
                }
            }
        }
    }

    return search;
}

int GetCollisionDirection(Rectangle r1, Rectangle r2) {
    float dx = r1.x - r2.x;
    float dy = r1.y - r2.y;
    float w = 0.5f * (r1.width + r2.width);
    float h = 0.5f * (r1.height + r2.height);
    float wy = w * dy;
    float hx = h * dx;

    if (wy > hx) {
        return (wy + hx > 0) ? 0 : 1;
    } else {
        return (wy + hx > 0) ? 2 : 3;
    }
}

void HandleCollisions(CollisionGrid& grid, std::vector<Player>& players, std::vector<Vector2>& vels) {
    for (auto& i : grid.colls_stage) {
        int p1 = i.first;
        StageCell sc = i.second;
        Rectangle r1 = {players[p1].rect1.x, players[p1].rect2.y, (float)grid.cell_size, (float)grid.cell_size * 2.0f};
        int dir = GetCollisionDirection(r1, sc.rect);
        switch (dir) {
            case 0:
                vels[p1].x *= -1;
                break;
            case 1:
                vels[p1].y *= -1;
                break;
            case 2:
                vels[p1].x *= -1;
                break;
            case 3:
                vels[p1].y *= -1;
                break;
        }
    }
    for (auto& i : grid.colls) {
        int p1 = i.first;
        int p2 = i.second;
        // AABB , combine both both player rectangles
        Rectangle r1 = {players[p1].rect1.x, players[p1].rect2.y, (float)grid.cell_size, (float)grid.cell_size * 2.0f};
        Rectangle r2 = {players[p2].rect1.x, players[p2].rect2.y, (float)grid.cell_size, (float)grid.cell_size * 2.0f};
        if (CheckCollisionRecs(r1, r2)) {
            // Handle collision
            // For now just reverse velocities
            std::cout << "coll" << std::endl;
            vels[p1].x *= -1;
            vels[p1].y *= -1;
            vels[p2].x *= -1;
            vels[p2].y *= -1;
            // this wont really work, need to normalize velocity vectors
            // and have it move away from each other
          
            
            break;
        }
    }
}

