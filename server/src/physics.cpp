#include <physics.hpp>


Rectangle PlayerPosToRect(Vector2int player_position, float scale, int player_width, int player_height) {
    Rectangle player_rect;
    player_rect.x = uint16_t((float)player_position.x - ((player_width * scale) / 6.0f));
    player_rect.y = uint16_t((float)player_position.y - ((player_height * scale) / 6.0f) - (20.0f * scale / 6.0f));
    player_rect.width = uint16_t((player_width * scale) / 3.0f);
    player_rect.height = uint16_t((player_height * scale) / 2.0f);
    return player_rect;
}

bool RectRectCollision(Rectangle rect1, Rectangle rect2) {
    if ((rect1.x < (rect2.x + rect2.width) && (rect1.x + rect1.width) > rect2.x) &&
        (rect1.y < (rect2.y + rect2.height) && (rect1.y + rect1.height) > rect2.y)){
        return true;
    }
    return false;
}

int RectRectCollisionDirection(Rectangle rect1, Rectangle rect2) {
    if (!((rect1.x < (rect2.x + rect2.width) && (rect1.x + rect1.width) > rect2.x) &&
          (rect1.y < (rect2.y + rect2.height) && (rect1.y + rect1.height) > rect2.y))) {
        return 0;
    }
    
    float center1X = rect1.x + rect1.width/2;
    float center1Y = rect1.y + rect1.height/2;
    float center2X = rect2.x + rect2.width/2;
    float center2Y = rect2.y + rect2.height/2;
    
    float dx = center1X - center2X;
    float dy = center1Y - center2Y;
    
    float absDx = abs(dx);
    float absDy = abs(dy);
    
    if (absDx > absDy) {
        if (dx > 0) return 2;  // right collision
        return 1;              // left collision
    } else {
        if (dy > 0) return 4;  // bottom collision
        return 3;              // top collision
    }
}

