#include <item.hpp>


Item::Item() {

}

void Item::Draw() {
    DrawTexturePro(anim_data.tex, draw_data.source, draw_data.dest, position, 0.0f, WHITE);
}

void Item::LoadTextures(std::string item) {
    try {
        anim_data.img = LoadImageAnim(item.c_str(), &anim_data.fc);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    anim_data.tex = LoadTexture(item.c_str());
}
void Item::Update(Vector2int player_pos, DrawData draw_data) {
    this->draw_data.dest.x = V2intToV2(player_pos).x;
    this->draw_data.dest.y = V2intToV2(player_pos).y;
    this->draw_data.dest.width = this->anim_data.img.width * draw_data.scale;
    this->draw_data.dest.height = this->anim_data.img.height * draw_data.scale;
    position = V2intToV2(player_pos);
}