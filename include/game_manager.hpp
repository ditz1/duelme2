#pragma once
#include <globals.hpp>
#include <player.hpp>
#include <helpers.hpp>
#include <networking.hpp>

void InitGameState(GameState* game);
void ParseGameState(GameState* game, Connection* conn, Player* player);
void SendGameStateRequest(GameState* game, Connection* conn);
void RequestStateUpdate(GameState* game, Connection* conn, Player* player);
void DrawGameState(GameState* game);