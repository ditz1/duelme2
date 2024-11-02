#include "stage_cell.hpp"
#include <string>
#include <iostream>
#include <fstream>


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

        if (stage_chars[i] == 'R') {
            cell.color = RED;
        } else if (stage_chars[i] == 'G') {
            cell.color = GREEN;
        } else if (stage_chars[i] == 'B') {
            cell.color = BLUE;
        } else {
            cell.color = BLACK;
        }
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
    int width = 800;
    int height = 450;
    InitWindow(width, height, "stage testing");
    SetTargetFPS(60);

    int cell_size = 50;
    std::string stage_file = "stage_1.txt";
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

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
            DrawStage(stage, cell_size);
            DrawStageLines(stage, cell_size);
            DrawText("Stage Testing", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();


}