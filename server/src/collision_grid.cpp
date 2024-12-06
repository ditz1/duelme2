#include "collision_grid.hpp"
#include <iostream>
#include <set>

void HandleCollisions(CollisionGrid& grid, GameState& game_state, std::vector<GridCoords>& search) {
    std::cout << search.size() << std::endl;
    std::cout << grid.colls.size() << std::endl;
    std::cout << grid.colls_stage.size() << std::endl;
    std::cout << grid.occupied_cells.size() << std::endl;

}

void GenerateCollisionGrid(CollisionGrid &grid, int cell_size, int rows, int cols){
    std::cout << "generating" << std::endl;
    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    std::cout << "cell_size: " << cell_size << std::endl;

    grid.cell_size = cell_size;
    grid.rows = rows;    
    grid.cols = cols;
    std::cout << "1" << std::endl;

    grid.cells.resize(rows);
    for (int i = 0; i < rows; i++) {
        grid.cells[i].resize(cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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




void UpdateCollisionGrid(CollisionGrid &grid, std::vector<Player> &players){
    grid.occupied_cells.clear();
    //std::cout << grid.cells[0].size() << std::endl;
    //std::cout << grid.cells.size() << std::endl;
    //std::cout << grid.max_x << ", " << grid.max_y << std::endl;

    for (auto& i : players){
        int x1 = floor((i.rect1.x + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        int y1 = floor((i.rect1.y + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (i.rect1.x >= grid.max_y) x1 = grid.cells[0].size() - 1;
        if (i.rect1.y >= grid.max_x) y1 = grid.cells.size() - 1;
        // x, y is top left cell adjacent of player
        //std::cout << "x1: " << x1 << " y1: " << y1 << std::endl;
        grid.cells[y1][x1].is_occupied = true;
        grid.occupied_cells.push_back({x1, y1, i.id});

        int x2 = floor((i.rect2.x + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        int y2 = floor((i.rect2.y + ((float)grid.cell_size / 2.0f)) / grid.cell_size);
        if (x2 < 0) x2 = 0;
        if (y2 < 0) y2 = 0;
        if (i.rect2.x >= grid.max_y) x2 = grid.cells[0].size() - 1;
        if (i.rect2.y >= grid.max_x) y2 = grid.cells.size() - 1;
        //std::cout << "x1: " << x1 << " y1: " << y1 << std::endl;

        grid.cells[y2][x2].is_occupied = true;
        grid.occupied_cells.push_back({x2, y2, i.id});
    }
}

std::vector<GridCoords> GetCollisionSearch(CollisionGrid& grid) {
    grid.colls.clear();
    grid.colls_stage.clear();
    std::vector<GridCoords> search;
    std::set<std::pair<int, int>> uniqueCells; // to prevent duplicates
    
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



// this function was made for local testing, should not be used here. the Player is not the same, dead code
// void HandleCollisions(CollisionGrid& grid, std::vector<Player>& players, std::vector<Vector2>& vels) {
//     for (auto& i : grid.colls_stage) {
//         int p1 = i.first;
//         StageCell sc = i.second;
//         Rectangle r1 = {static_cast<uint16_t>(players[p1].rect1.x), static_cast<uint16_t>(players[p1].rect2.y), 
//                         static_cast<uint16_t>(grid.cell_size), static_cast<uint16_t>(grid.cell_size * 2.0f)};
//         int dir = GetCollisionDirection(r1, sc.rect);
//         switch (dir) {
//             case 0:
//                 vels[p1].x *= -1;
//                 break;
//             case 1:
//                 vels[p1].y *= -1;
//                 break;
//             case 2:
//                 vels[p1].x *= -1;
//                 break;
//             case 3:
//                 vels[p1].y *= -1;
//                 break;
//         }
//     }
//     for (auto& i : grid.colls) {
//         int p1 = i.first;
//         int p2 = i.second;
//         // AABB , combine both both player rectangles
//         Rectangle r1 = {static_cast<uint16_t>(players[p1].rect1.x), static_cast<uint16_t>(players[p1].rect2.y), static_cast<uint16_t>(grid.cell_size), static_cast<uint16_t>(grid.cell_size * 2.0f)};
//         Rectangle r2 = {static_cast<uint16_t>(players[p2].rect1.x), static_cast<uint16_t>(players[p2].rect2.y), static_cast<uint16_t>(grid.cell_size), static_cast<uint16_t>(grid.cell_size * 2.0f)};
//         if (RectRectCollision(r1, r2)) {
//             // Handle collision
//             // For now just reverse velocities
//             std::cout << "coll" << std::endl;
//             vels[p1].x *= -1;
//             vels[p1].y *= -1;
//             vels[p2].x *= -1;
//             vels[p2].y *= -1;
//             // this wont really work, need to normalize velocity vectors
//             // and have it move away from each other
          
            
//             break;
//         }
//     }
// }

