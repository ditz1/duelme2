#include <camera_manager.hpp>

void AdjustCameraPosition(std::array<Player, 4>& players, Camera2D& camera) {
    switch (num_players_connected) {
        case 2:
            AdjustTwoPlayerCamera(players, camera);
            break;
        default:
            break;
    }
}

void AdjustTwoPlayerCamera(std::array<Player,4> &players, Camera2D &camera) {
    Vector2 p1_pos = V2intToV2(players[0].Position());
    Vector2 p2_pos = V2intToV2(players[1].Position());
    Vector2 center = {0, 0};

    if (p1_pos.x >= p2_pos.x) {
        center.x = ((p1_pos.x + p2_pos.x) / 2); 
    } else if (p1_pos.x < p2_pos.x) {
        center.x = ((p2_pos.x + p1_pos.x) / 2);
    }
    if (p1_pos.y >= p2_pos.y) {
        center.y = ((p1_pos.y + p2_pos.y) / 2);
    } else if (p1_pos.y < p2_pos.y) {
        center.y = ((p2_pos.y + p1_pos.y) / 2);
    }

    camera.target = center;
    if (camera.target.x <= 0) {
        camera.target.x = 0;
    }
    if (camera.target.y <= 0) {
        camera.target.y = 0;
    }
    
}
