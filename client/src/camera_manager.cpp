#include <camera_manager.hpp>

void AdjustCameraPosition(std::array<Player, 4>& players, Camera2D& camera, int max_y_level, Vector2 center_stage) {
   
    switch (num_players_connected) {
        case 2:
            AdjustTwoPlayerCamera(players, camera, center_stage);
            break;
        default:  
            AdjustOnePlayerCamera(players, camera, center_stage);
            break;
    }
    if (camera.target.y > max_y_level) {
        camera.target.y = max_y_level;
    }
}
void AdjustOnePlayerCamera(std::array<Player,4> &players, Camera2D &camera, Vector2 center_stage) {
    Vector2 p1_pos = V2intToV2(players[0].Position());
    Vector2 center = {0, 0};    
    
    center.x = (p1_pos.x);
    center.y = (p1_pos.y);
    
    float smoothing = 0.1f;
    // if camera is very far away, just send it to where it needs to be
    if (std::abs(center.x - camera.target.x) > 100){ smoothing = 0.5f; }

    camera.target.x = camera.target.x + (center.x - camera.target.x) * smoothing;
    camera.target.y = camera.target.y + (center.y - camera.target.y) * smoothing;
    camera.target.x = std::max(0.0f, camera.target.x);
    camera.target.y = std::max(0.0f, camera.target.y);
}

void AdjustTwoPlayerCamera(std::array<Player,4> &players, Camera2D &camera, Vector2 center_stage) {
    Vector2 p1_pos = V2intToV2(players[0].Position());
    Vector2 p2_pos = V2intToV2(players[1].Position());
    Vector2 p_center = {0, 0};    
    Vector2 center = {0, 0};
    
    p_center.x = (p1_pos.x + p2_pos.x) / 2.0f;
    p_center.y = (p1_pos.y + p2_pos.y) / 2.0f;
    // so this is fine, but we also probably want to have a bias towards the middle of the stage
    // midpoint of line from center to center_stage
    center.x = (p_center.x + center_stage.x) / 2.0f;
    center.y = (p_center.y + center_stage.y) / 2.0f;

    
    float smoothing = 0.1f;
    if (std::abs(center.x - camera.target.x) > 100){ smoothing = 0.5f;}

    camera.target.x = camera.target.x + (center.x - camera.target.x) * smoothing;
    camera.target.y = camera.target.y + (center.y - camera.target.y) * smoothing;
    camera.target.x = std::max(0.0f, camera.target.x);
    camera.target.y = std::max(0.0f, camera.target.y);
}

// void AdjustTwoPlayerCamera(std::array<Player,4> &players, Camera2D &camera) {
//     Vector2 p1_pos = V2intToV2(players[0].Position());
//     Vector2 p2_pos = V2intToV2(players[1].Position());
//     Vector2 center = {0, 0};
//     int x1 = camera.target.x;
//     int y1 = camera.target.y;
//     if (p1_pos.x >= p2_pos.x) {
//         center.x = ((p1_pos.x + p2_pos.x) / 2); 
//     } else if (p1_pos.x < p2_pos.x) {
//         center.x = ((p2_pos.x + p1_pos.x) / 2);
//     }
//     if (p1_pos.y >= p2_pos.y) {
//         center.y = ((p1_pos.y + p2_pos.y) / 2);
//     } else if (p1_pos.y < p2_pos.y) {
//         center.y = ((p2_pos.y + p1_pos.y) / 2);
//     }
//     if (std::abs(x1 - center.x) > 1) {
//         camera.target = center;
//     }
//     if (std::abs(y1 - center.y) > 1) {
//         camera.target = center;
//     }
//     //camera.target = center;
//     if (camera.target.x <= 0) {
//         camera.target.x = 0;
//     }
//     if (camera.target.y <= 0) {
//         camera.target.y = 0;
//     } 
// }
