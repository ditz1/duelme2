#pragma once
#include "raylib.h"
static float dt = 0.05f;

typedef class Player {
public:
    Player() {
        color = {(unsigned char)GetRandomValue(0, 255), 
                 (unsigned char)GetRandomValue(0, 255), 
                 (unsigned char)GetRandomValue(0, 255), 255};
    }
    Vector2 pos;
    Vector2 vel; 
    Rectangle rect;
    Color color;

    void Update(){
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        if (pos.x <= 0 || pos.x + rect.width > 1280) { vel.x *= -1; }
        if (pos.y <= 0 || pos.y + rect.height > 720) { vel.y *= -1; }
        rect.x = pos.x;
        rect.y = pos.y;
    }
    void Draw() {
        DrawRectangleRec(rect, color);
    }
} Player;
