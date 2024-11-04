#pragma once

#include <globals.hpp>
#include <player.hpp>

void AdjustCameraPosition(std::array<Player, 4>& players, Camera2D& camera);
void AdjustTwoPlayerCamera(std::array<Player, 4>& players, Camera2D& camera);
void AdjustThreePlayerCamera(std::array<Player, 4>& players, Camera2D& camera);
void AdjustFourPlayerCamera(std::array<Player, 4>& players, Camera2D& camera);