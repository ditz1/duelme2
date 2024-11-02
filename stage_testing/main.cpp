#include "stage_cell.hpp"
#include <string>
#include <iostream>
#include <fstream>

int LoadStageFile(std::string file_path, std::vector<StageCell> &stage) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << file_path << std::endl;
        return 1;
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

    // first load stage file to get rows and cols
    while (file.get(c)) {
        if (c == '!' && stage_started) stage_ended = true;
        if (c == '!' && !stage_started) stage_started = true;
        if (stage_started) {
            if (c == '\n') rows++;
            if (rows >= 0) num_chars++;
            for (char rc : reserved_chars) {
                if (c != rc) {
                    stage_chars.push_back(c);
                    break;
                }
            }
        }
        // this needs to be at end
        if (stage_started && stage_ended) {
            stage_is_valid = true; 
            break;
        }
    }
    if (!stage_is_valid) {
        std::cerr << "Error: invalid stage file " << file_path << std::endl;
        return 1;
    } else {
        std::cout << std::endl;
        file.close();
    }
    cols = (num_chars / rows) - 1;
    
    // then load into stage
    std::vector<StageCell> stage_temp(rows * cols);
    for (char c : stage_chars) {
        std::cout << c;
    }






    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    std::cout << "num_chars: " << num_chars << std::endl;

    return 0;
}


int main() {
    int width = 800;
    int height = 450;
    InitWindow(width, height, "stage testing");
    SetTargetFPS(60);

    int grid_width = 40;
    int grid_height = 20;
    int cell_size = 0;

    std::vector<StageCell> stage;

    while (width % grid_width != 0) {
        grid_width--;
    }
    while (height % grid_height != 0) {
        grid_height--;
    }
    if (grid_width > grid_height) {
        cell_size = width / grid_width;
    } else {
        cell_size = height / grid_height;
    }

    if (LoadStageFile("stage_1.txt", stage)){
        return 1;
    }


    std::cout << "grid_width: " << grid_width << std::endl;
    std::cout << "grid_height: " << grid_height << std::endl;
    std::cout << "cell_width: " << cell_size << std::endl;
    


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
            DrawText("Stage Testing", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();


}