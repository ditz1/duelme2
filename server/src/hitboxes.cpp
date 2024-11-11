#include <hitboxes.hpp>

Rectangle GeneratePlayerHitboxPunch(Rectangle rect, int face){
    Rectangle hitbox;
    uint16_t x;
    uint16_t w;
    uint16_t y = rect.y;
    uint16_t h = rect.height;

    if (face > 0){ // facing right
        x = uint16_t((float)rect.x + ((float)(rect.width) / 2.0f)); 
        w = uint16_t(((float)rect.width * 0.75f));
    } else {
        x = uint16_t((float)rect.x - ((float)(rect.width) / 4)); 
        w = uint16_t((float)rect.width * 0.75f);
    }

    hitbox = {x, y, w, h};
    return hitbox;
}

Rectangle GeneratePlayerHitboxKick(Rectangle rect, float scale, int face){
    Rectangle hitbox;
    uint16_t x;
    uint16_t w;
    uint16_t y = rect.y;
    uint16_t h = rect.height;

    if (face > 0){ // facing right
        x = uint16_t((float)rect.x + ((float)(rect.width) / 2.0f)); 
        w = uint16_t(((float)rect.width * 0.75f) + (scale * 2.0f));
    } else {
        x = uint16_t((float)rect.x - ((float)(rect.width) / 4) - (scale * 2.0f)); 
        w = uint16_t((float)rect.width * 0.75f);
    }

    hitbox = {x, y, w, h};
    return hitbox;
}


