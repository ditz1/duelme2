#include <item.hpp>


Item::Item() {

}

void Item::Draw() {
    DrawTextureEx(anim_data.tex, position, 0.0f, draw_data.scale, WHITE);
    DrawTexturePro(anim_data.tex, draw_data.source, draw_data.dest, position, 0.0f, WHITE);
}

void Item::LoadTextures(std::string str_item) {
    try {
        anim_data.img = LoadImage(str_item.c_str());
        ImageResize(&anim_data.img, 16, 16);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    anim_data.tex = LoadTextureFromImage(anim_data.img);    
}
void Item::Update(Vector2int player_pos, DrawData draw_data) {
    this->draw_data.dest.x = V2intToV2(player_pos).x;
    this->draw_data.dest.y = V2intToV2(player_pos).y;

    // draw_data.source = {0.0f, 0.0f, (float)texs[0].img.width, (float)texs[0].img.height};
    // draw_data.dest = {0.0f, 0.0f, (float)texs[0].img.width * draw_data.scale, (float)texs[0].img.height * draw_data.scale};
    this->draw_data.source.width = 32;
    this->draw_data.source.height = 32;
    position = V2intToV2(player_pos);
}