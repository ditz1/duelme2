#include <item.hpp>


Item::Item() {

}

void Item::Draw() {
    //DrawTextureEx(anim_data.tex, position, 0.0f, draw_data.scale, WHITE);
    DrawTexturePro(anim_data.tex, draw_data.source, draw_data.dest, position, 0.0f, WHITE);
}

void Item::LoadTextures(std::string str_item) {
    try {
        anim_data.img = LoadImage(str_item.c_str());
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    anim_data.tex = LoadTextureFromImage(anim_data.img);  
    draw_data.source = {0.0f, 0.0f, (float)anim_data.img.width, (float)anim_data.img.height};
    draw_data.dest = {0.0f, 0.0f, (float)anim_data.img.width * 1.75f, (float)anim_data.img.height * 1.75f,};  
}
void Item::Update(Vector2int player_pos, DrawData draw_data) {
  
    position = V2intToV2(player_pos);

    position.x -= (((float)anim_data.tex.width / 1.25f));
    position.y -= (((float)anim_data.tex.height / 2.0f));

    if (face_dir > 0 && this->draw_data.source.width < 0) {
        this->draw_data.source.width *= -1;
    } else if (face_dir < 0 && this->draw_data.source.width > 0) {
        this->draw_data.source.width *= -1;
    }
    if (face_dir > 0){
        position.x += 8.0f;
    } else {
        position.x -= 8.0f;
    }

    position.x *= -1.0f;
    position.y *= -1.0f;
   
}