#pragma once
#include "globals.hpp"
#include "player.hpp"
#include <algorithm>

void DrawDebugInfo(GameState game, Player& player, std::array<Player, 4> all_players);
void StripStageString(std::string& stage_str);
void LogGameState(GameState game, Connection* conn);