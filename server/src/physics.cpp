#include <physics.hpp>


int gravity = 4;
int velocity = 5;

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

CollisionIndex RectRectCollisionDirection(Rectangle rect1, Rectangle rect2) {
    CollisionIndex  direction = {false, false, false, false, false, false, false, false};
    bool& right_t  = direction[0];
    bool& right_b   = direction[1];
    bool& left_t   = direction[2];
    bool& left_b   = direction[3];
    bool& top_l    = direction[4];
    bool& top_r    = direction[5];
    bool& bottom_l = direction[6];
    bool& bottom_r = direction[7];

    bool is_colliding = (rect1.x < rect2.x + rect2.width &&
                        rect1.x + rect1.width > rect2.x &&
                        rect1.y < rect2.y + rect2.height &&
                        rect1.y + rect1.height > rect2.y);

    if (!is_colliding) {
        return direction;
    }

    float overlap_x = std::min(rect1.x + rect1.width, rect2.x + rect2.width) - 
                     std::max(rect1.x, rect2.x);
    float overlap_y = std::min(rect1.y + rect1.height, rect2.y + rect2.height) - 
                     std::max(rect1.y, rect2.y);

    bool left_bottom_colliding = ((rect1.x < rect2.x + rect2.width && 
                                 rect1.x > rect2.x && 
                                 rect1.y + rect1.height > rect2.y) || 
                                 (rect1.x - 2 < rect2.x + rect2.width && 
                                 rect1.x + 2 > rect2.x && 
                                 rect1.y + rect1.height > rect2.y));
    bool right_bottom_colliding = ((rect1.x + rect1.width < rect2.x + rect2.width && 
                                  rect1.x + rect1.width > rect2.x && 
                                  rect1.y + rect1.height > rect2.y) || 
                                 (rect1.x - 2 < rect2.x + rect2.width && 
                                 rect1.x + 2 > rect2.x && 
                                 rect1.y + rect1.height > rect2.y));
    bool left_top_colliding = (rect1.x < rect2.x + rect2.width && 
                              rect1.x > rect2.x && 
                              rect1.y < rect2.y + rect2.height);
    bool right_top_colliding = (rect1.x + rect1.width < rect2.x + rect2.width && 
                               rect1.x + rect1.width > rect2.x && 
                               rect1.y < rect2.y + rect2.height);
    
    bool top_right_colliding = (rect1.x + rect1.width > rect2.x && 
                               rect1.x + rect1.width < rect2.x + rect2.width && 
                               rect1.y < rect2.y + rect2.height);
    bool top_left_colliding = (rect1.x > rect2.x && 
                              rect1.x < rect2.x + rect2.width && 
                              rect1.y < rect2.y + rect2.height);
    bool bottom_right_colliding = (rect1.x + rect1.width > rect2.x && 
                                  rect1.x + rect1.width < rect2.x + rect2.width && 
                                  rect1.y + rect1.height > rect2.y);
    bool bottom_left_colliding = (rect1.x > rect2.x && 
                                 rect1.x < rect2.x + rect2.width && 
                                 rect1.y + rect1.height > rect2.y);

    if (overlap_x < overlap_y) {
        if (rect1.x + rect1.width/2 < rect2.x + rect2.width/2) {
            if (top_right_colliding) right_t = true;
            if (bottom_right_colliding) right_b = true;
        } else {
            if (top_left_colliding) left_t = true;
            if (bottom_left_colliding) left_b = true;
        }
    } else {
        if (rect1.y + rect1.height/2 < rect2.y + rect2.height/2) {
            if (left_bottom_colliding) bottom_l = true;
            if (right_bottom_colliding) bottom_r = true;
        } else {
            if (left_top_colliding) top_l = true;
            if (right_top_colliding) top_r = true;
        }
    }

    return direction;
}

