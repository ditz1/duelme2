#include "stage_cell.hpp"
#include "spatial_grid.hpp"
#include "test_helpers.hpp"
#include <string>
#include <iostream>
#include <math.h>
#include <fstream>
#include <tuple>



void DrawStage(std::vector<StageCell> &stage, int cell_size) {
    for (StageCell cell : stage) {
        DrawRectangle(cell.rect.x, cell.rect.y, cell.rect.width, cell.rect.height, cell.color);
    }
}

void GenerateStage(std::vector<StageCell> &stage, int cell_size) {
    for (StageCell &cell : stage) {
        cell.rect.x = cell.grid_pos.x * cell_size;
        cell.rect.y = cell.grid_pos.y * cell_size;
        cell.rect.width = cell_size;
        cell.rect.height = cell_size;
    }
}

void DrawStageLines(std::vector<StageCell> &stage, int cell_size) {
    for (StageCell cell : stage) {
        DrawRectangleLinesEx(cell.rect, 2.0f, BLACK);
    }
}

std::tuple<int,int> LoadStageFile(std::string file_path, std::vector<StageCell> &stage) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << file_path << std::endl;
        return std::make_tuple(-1, -1);
    }

    char c;
    bool stage_started = false;
    bool stage_ended = false;
    bool stage_is_valid = false;

    int rows = -1;
    int cols = -1;
    int num_chars = 0;
    std::vector<char> stage_chars;
    char reserved_chars[] = {'!', ' ', '\n'};
    int longest_column = 0;
    int curr_longest_column = 0;
    // first load stage file to get rows and cols
    while (file.get(c)) {
        if (c == '!' && stage_started) stage_ended = true;
        if (c == '!' && !stage_started) stage_started = true;
        if (stage_started) {
            if (c == '\n') {
                rows++;
                if (curr_longest_column > longest_column) {
                    longest_column = curr_longest_column;
                }
                curr_longest_column = 0;
            } else {
                curr_longest_column++;
            
            } 
            if (rows >= 0) {
                num_chars++;
            }
            bool is_stage_char = true;
            for (char rc : reserved_chars) {
                if (c == rc) {
                    is_stage_char = false;
                    break;
                }
            }
            if (is_stage_char) stage_chars.push_back(c);
        }
        // this needs to be at end
        if (stage_started && stage_ended) {
            stage_is_valid = true; 
            break;
        }
    }
    
    std::cout << stage_chars.data() << std::endl;

    if (!stage_is_valid) {
        std::cerr << "Error: invalid stage file " << file_path << std::endl;
        return std::make_tuple(-1, -1);
    } else {
        file.close();
    }
    cols = longest_column;
    
    // then load into stage
    std::vector<StageCell> stage_temp(rows * cols);
    int grid_x = 0;
    int grid_y = -1;

    for (int i = 0; i < stage_chars.size(); i++) {
        
        if (i % cols == 0) {
            grid_x = 0;
            grid_y++;

            if (grid_y == rows) {
                break;
            } 
        }
        grid_x = i % cols;

        StageCell cell;
        cell.grid_pos = {(float)grid_x, (float)grid_y};

        // if (stage_chars[i] == 'R') {
        //     cell.color = RED;
        // } else if (stage_chars[i] == 'G') {
        //     cell.color = GREEN;
        // } else if (stage_chars[i] == 'B') {
        //     cell.color = BLUE;
        // } else {
        //     cell.color = BLACK;
        // }
        stage.push_back(cell);

    }

    std::cout << "longest_column: " << longest_column << std::endl;
    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    std::cout << "num_chars: " << num_chars << std::endl;

    std::cout << stage_chars.data() << std::endl;
    std::cout << "stage size: " << stage.size() << std::endl;

    return std::make_tuple(rows, cols);
}


int main(int argc, char* argv[]) {
    int width = 1280;
    int height = 720;
    InitWindow(width, height, "stage testing");
    SetTargetFPS(60);

    int cell_size = 50;
    std::string stage_file = "stage_2.txt";
    std::vector<StageCell> stage;

    if (argc > 1) stage_file = argv[1];

    std::tuple<int,int> stage_size = LoadStageFile(stage_file.c_str(), stage);
    if (std::get<0>(stage_size) == -1 || std::get<1>(stage_size) == -1) {
        std::cout << "Error: could not load stage file" << std::endl;
        return 1;
    }

    for (StageCell cell : stage) {
        std::cout << "grid_pos: " << cell.grid_pos.x << ", " << cell.grid_pos.y << std::endl;
    }

    if (std::get<0>(stage_size) == -1 || std::get<1>(stage_size) == -1) {
        return 1;
    } else {
        int tmp_width = width / std::get<1>(stage_size);
        int tmp_height = height / std::get<0>(stage_size);
        cell_size = tmp_width < tmp_height ? tmp_width : tmp_height;
    }
    std::cout << "cell_width: " << cell_size << std::endl;
    GenerateStage(stage, cell_size);
    int stage_width = std::get<1>(stage_size);
    int stage_height = std::get<0>(stage_size);
    std::cout << stage_height << " , "  << stage_width << std::endl;

    ////////////////////////////////////////////
    ////////////////////////////////////////////
    int p_width = cell_size;
    int p_height = cell_size * 2;
    int num_players = 10;
    std::vector<Player> players;
    for (int i = 0; i < num_players; i++){
        Player p = Player();
        p.pos = {float(i * 100.0f), float(10.0f * i)};
        p.rect.width = p_width;
        p.rect.height = p_height;
        p.vel = {float((unsigned char)GetRandomValue(0, 50)), float((unsigned char)GetRandomValue(0, 50))};
        players.push_back(p);
    }

    SpatialGrid grid;
    grid.Init(stage_width, stage_height, cell_size);
    std::cout << "Grid initialized with " << stage_width << "x" << stage_height 
          << " cells, total: " << (stage_width * stage_height) << std::endl;
    
    while (!WindowShouldClose()) {  
        grid.Clear();

        // Use pos consistently for both insert and collision checking
        for (Player &p : players) {
            float grid_x = p.pos.x / (float)cell_size;
            float grid_y = p.pos.y / (float)cell_size;
            grid.Insert(grid_x, grid_y, &p);
        }

        for (Player &p : players) {
            float grid_x = p.pos.x / (float)cell_size;
            float grid_y = p.pos.y / (float)cell_size;
            int i = floor(grid_x);
            int j = floor(grid_y);
            uint64_t hash = grid.HashCell(i, j);

            const auto& cells_at_pos = grid.GetCellsAt(hash);

            for (const Cell& cell : cells_at_pos) {
                if (cell.player != &p) {
                    // Check actual collision using rectangles
                    if (CheckCollisionRecs(p.rect, cell.player->rect)) {
                        printf("Collision between players at hash %ld\n", hash);
                        p.vel.x *= -1;
                        p.vel.y *= -1;
                    }
                }
            }

            p.Update();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawStage(stage, cell_size);
            DrawStageLines(stage, cell_size);
            DrawText("Stage Testing", 10, 10, 20, DARKGRAY);
            DrawFPS(10, 40);
            for (int i = 0; i < grid.Rows(); i++) {
                for (int j = 0; j < grid.Cols(); j++) {
                    DrawRectangleLines(j * cell_size, i * cell_size, cell_size, cell_size, GRAY);
                }
            }
            for (Player &p : players) {
                p.Draw();
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}