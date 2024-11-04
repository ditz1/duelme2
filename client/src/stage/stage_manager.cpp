#include <stage_manager.hpp>
#include <helpers.hpp>
// 7 x 5
std::string STAGE_1 = "RGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRG";

Stage::Stage() {
    rows = 0;
    cols = 0;
    cell_size = 50;
    current_stage_id = 0;
    is_loaded = false;
    should_regenerate = false;
}

Stage::Stage(int rows, int cols, int cell_size) {
    this->rows = rows;
    this->cols = cols;
    this->cell_size = cell_size;
    current_stage_id = 0;
    is_loaded = false;
    should_regenerate = false;
}

void Stage::Draw() {
    for (StageCell cell : cells) {
        cell.Draw();
    }
}

void Stage::DrawLines() {
    for (StageCell cell : cells) {
        DrawRectangleLinesEx(cell.rect, 2.0f, BLACK);
    }
}

// MUST BE CALLED AFTER LOAD
void Stage::Generate() {
    if (!is_loaded) {
        std::cerr << "Error: stage not loaded" << std::endl;
        return;
    }

    int width = GetScreenWidth() / cols;
    int height = GetScreenHeight() / rows;
    cell_size = width < height ? width : height;

    for (StageCell &cell : cells) {
        cell.rect.x = cell.grid_location.x * cell_size;
        cell.rect.y = cell.grid_location.y * cell_size;
        cell.rect.width = cell_size;
        cell.rect.height = cell_size;
    }
}

void Stage::FillGrid() {
    for (int i = 0; i < rows * cols; i++) {
        StageCell cell;
        cell.grid_location = {(float)(i % cols), (float)(i / cols)};
        cell.rect = {0, 0, 0, 0};
        cell.color = WHITE;
        cells.push_back(cell);
    }
}

void Stage::LoadFromString(std::string stage_str) {
    if (rows != 0) rows = 0;
    if (cols != 0) cols = 0;

    for (char c : stage_str) {        
        if (c == '\n') {
            rows++;
        }
        if (c == '\r') {
            cols++;
        }
    }

    StripStageString(stage_str);
        
    int grid_x = 0;
    int grid_y = -1;


    for (int i = 0; i < stage_str.size(); i++) {
        
        if (i % cols == 0) {
            grid_x = 0;
            grid_y++;

            if (grid_y == rows) {
                break;
            } 
        }
        grid_x = i % cols;

        StageCell cell;
        cell.grid_location = {(float)grid_x, (float)grid_y};

        if (stage_str[i] == 'R') {
            cell.color = PURPLE;
        } else if (stage_str[i] == 'G') {
            cell.color = GREEN;
        } else if (stage_str[i] == 'B') {
            cell.color = BLUE;
        } else if (stage_str[i] == '#') {
            cell.color = BLACK;
        } else {
            cell.color = WHITE;
        }
        cells.push_back(cell);
    }
    is_loaded = true;
}


void Stage::LoadFromFile(std::string file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << file_path << std::endl;
        return;
    }

    std::string stage_str;
    char c;
    while (file.get(c)){
        stage_str += c;
    }
    LoadFromString(stage_str);

    file.close();
}