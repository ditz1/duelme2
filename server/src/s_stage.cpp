#include <s_stage.hpp>
#include <collision_grid.hpp>

void ServerStage::LoadDataIntoCells() {
    // clear original data

    CollisionGrid grid;
    collision_grid = grid;
    max_y_level = 1000000;
    max_x_level = 1000000;
    min_y_level = 0;
    min_x_level = 0;
    int max_rect_width = 0;
    int max_rect_height = 0;
    int j = 0;
    for (size_t i = 0; i < data.size(); i+=8){
        if (data[i] == 0xFF && data[i+1] == 0xFF && data[i+2] == 0xFF && data[i+3] == 0xFF){
            j = i + 4;
            break;
        }
        U16ToU8 x;
        x.b.b1 = data[i];
        x.b.b0 = data[i+1];
        U16ToU8 y;
        y.b.b1 = data[i+2];
        y.b.b0 = data[i+3];
        U16ToU8 width;
        width.b.b1 = data[i+4];
        width.b.b0 = data[i+5];
        U16ToU8 height;
        height.b.b1 = data[i+6];
        height.b.b0 = data[i+7];

        Rectangle rect;
        rect.x = x.u;
        rect.y = y.u;
        rect.width = width.u;
        rect.height = height.u;
        if (rect.width > max_rect_width) max_rect_width = rect.width;
        if (rect.height > max_rect_height) max_rect_height = rect.height;
        cells.push_back(rect);
    }
    std::vector<int> x1;
    std::vector<int> x2;
    std::vector<int> y1;
    std::vector<int> y2;
    for (Rectangle cell : cells){
        x1.push_back(cell.x);
        x2.push_back(cell.x + cell.width);
        y1.push_back(cell.y);
        y2.push_back(cell.y + cell.height);
    }
    max_x_level = *std::max_element(x1.begin(), x1.end());
    min_x_level = *std::min_element(x2.begin(), x2.end());
    max_y_level = *std::max_element(y1.begin(), y1.end());
    min_y_level = *std::min_element(y2.begin(), y2.end());

    std::cout << "max x: " << max_x_level << " max y: " << max_y_level << " min x: " << min_x_level << " min y: " << min_y_level << std::endl;

    printf("\n");

    scale = (float)(data[j]) / 10.0f;
    U16ToU8 width;
    width.b.b0 = data[j+1];
    width.b.b1 = data[j+2];
    player_width = width.u;
    U16ToU8 height;
    height.b.b1 = data[j+2];
    height.b.b0 = data[j+3];
    player_height = height.u;
    int rows = (max_y_level - min_y_level) / max_rect_height;
    int cols = (max_x_level - min_x_level) / max_rect_width;


    // generate collision grid
    GenerateCollisionGrid(collision_grid, max_rect_width, rows, cols);
    std::cout << collision_grid.cell_size << ", " << collision_grid.rows << ", " << collision_grid.cols << std::endl;
    

}


bool ServerStage::ProcessPlayerCollision(Vector2int player_position) {
    // from Player::Update
    // brute force for now

    Rectangle player_rect;
    player_rect.x = uint16_t((float)player_position.x - ((float)player_width * scale / 6.0f));
    player_rect.y = uint16_t((float)player_position.y - ((float)player_height * scale / 6.0f) - ((5.0f * scale) * (scale / 6.0f)));
    player_rect.width = uint16_t((float)player_width * scale / 3.0f);
    player_rect.height = uint16_t((float)player_height * scale / 2.0f);


    for (Rectangle cell : cells){
        if (RectRectCollision(player_rect, cell)){
            return true;
            break;
        }
    }
    return false;
}

CollisionIndex ServerStage::ProcessPlayerCollisionDirection(Vector2int player_position) {
    // from Player::Update
    // brute force for now
    Rectangle player_rect;
    player_rect.x = uint16_t((float)player_position.x - ((float)player_width * scale / 6.0f));
    player_rect.y = uint16_t((float)player_position.y - ((float)player_height * scale / 6.0f) - ((5.0f * scale) * (scale / 6.0f)));
    player_rect.width = uint16_t((float)player_width * scale / 3.0f);
    player_rect.height = uint16_t((float)player_height * scale / 2.0f);
    CollisionIndex direction = {false, false, false, false, false, false, false, false};
    for (Rectangle cell : cells){
        CollisionIndex cell_coll_dir = RectRectCollisionDirection(player_rect, cell);
        for (int i = 0; i < 8; i++){                    
            direction[i] |= cell_coll_dir[i];
        }
    }
    return direction;
}
