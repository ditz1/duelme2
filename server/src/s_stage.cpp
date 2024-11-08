#include <s_stage.hpp>

void ServerStage::LoadDataIntoCells() {
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
        cells.push_back(rect);
    }

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

}


bool ServerStage::ProcessPlayerCollision(Vector2int player_position) {
    // from Player::Update
    // brute force for now

    Rectangle player_rect;
    player_rect.x = uint16_t((float)player_position.x - ((float)player_width * scale / 6.0f));
    player_rect.y = uint16_t((float)player_position.y - ((float)player_height * scale / 6.0f) - (20.0f * scale / 6.0f));
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

std::tuple<bool,bool,bool,bool> ServerStage::ProcessPlayerCollisionDirection(Vector2int player_position) {
    // from Player::Update
    // brute force for now
    Rectangle player_rect;
    player_rect.x = uint16_t((float)player_position.x - ((float)player_width * scale / 6.0f));
    player_rect.y = uint16_t((float)player_position.y - ((float)player_height * scale / 6.0f) - (20.0f * scale / 6.0f));
    player_rect.width = uint16_t((float)player_width * scale / 3.0f);
    player_rect.height = uint16_t((float)player_height * scale / 2.0f);
    std::tuple<bool,bool,bool,bool> direction = {false, false, false, false};

    for (Rectangle cell : cells){
        switch(RectRectCollisionDirection(player_rect, cell)){
            case 1: std::get<0>(direction) = true; break;
            case 2: std::get<1>(direction) = true; break;
            case 3: std::get<2>(direction) = true; break;
            case 4: std::get<3>(direction) = true; break;
            default: break;
        }
    }
    return direction;
}
