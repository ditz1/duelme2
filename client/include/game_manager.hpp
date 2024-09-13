#pragma once
#include <globals.hpp>
#include <player.hpp>
#include <helpers.hpp>
#include <networking.hpp>

void InitGameState(GameState* game);
void ParseGameState(GameState* game, Connection* conn, Player* player);
void ParseLobbyState(GameState* game);
void ParseEndState(GameState* game, Connection* conn, Player* player);
void ParseAssignPlayerId(GameState* game, Connection* conn, Player* player);
void SendReadyRequest(Player* player, Connection* conn);
void SendRestartRequest(GameState* game, Connection* conn, Player* player);
void SendGameStateRequest(GameState* game, Connection* conn);
void RequestStateUpdate(GameState* game, Connection* conn, Player* player);
void DrawGameState(GameState* game);
void DrawLobbyState(GameState* game);