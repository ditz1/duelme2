#pragma once

#include <globals.hpp>
#include <player.hpp>

void AdjustCameraPosition(std::array<Player, 4>& players, Camera2D& camera, int max_y_level, Vector2 center_stage);
void AdjustOnePlayerCamera(std::array<Player, 4>& players, Camera2D& camera, Vector2 center_stage);
void AdjustTwoPlayerCamera(std::array<Player, 4>& players, Camera2D& camera, Vector2 center_stage);
void AdjustThreePlayerCamera(std::array<Player, 4>& players, Camera2D& camera);
void AdjustFourPlayerCamera(std::array<Player, 4>& players, Camera2D& camera);