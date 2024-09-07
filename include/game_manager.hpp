#pragma once
#include <globals.hpp>
#include <player.hpp>

void InitGameState(GameState* game);
void ParseGameState(GameState* game, Connection* conn, Player* player);
void SendGameState(GameState* game, Connection* conn);