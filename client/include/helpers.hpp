#pragma once
#include "globals.hpp"
#include "player.hpp"

void DrawDebugInfo(GameState game, Player& player, std::array<Player, 4> all_players);
void LogGameState(GameState game, Connection* conn);