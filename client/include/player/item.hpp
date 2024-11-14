#pragma once
#include <player.hpp>


typedef class Item {
public:
    Item();
    ~Item();
    void Draw();
    void Update(Vector2int player_pos, DrawData draw_data);
    void LoadTexture(std::string item);
    DrawData draw_data;
    AnimData anim_data;
    Vector2 position;
    ItemType type;
    int player_assigned;
    int face_dir;
} Item;