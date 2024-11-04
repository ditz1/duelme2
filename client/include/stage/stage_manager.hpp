#pragma once

#include <stage_cell.hpp>
#include <tuple>
#include <fstream>

extern std::string STAGE_1;

typedef class Stage {
public:
    Stage();
    Stage(int rows, int cols, int cell_size);
    void Draw();
    void DrawLines();
    void Generate();
    void FillGrid();
    void LoadFromFile(std::string file_path);
    void LoadFromString(std::string stage_str);
    std::vector<StageCell> cells;
    int rows;
    int cols;
    int cell_size;
    int current_stage_id;
    bool is_loaded;
    bool should_regenerate;
private:
} Stage;