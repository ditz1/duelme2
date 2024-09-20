#pragma once
#include <globals.hpp>
#include <player.hpp>
#include <helpers.hpp>
#include <networking.hpp>
#include <player_anims.hpp>

void InitGameState(GameState* game);
void ParseGameState(GameState* game, Connection* conn, Player* player);
void ParseLobbyState(GameState* game);
void ParseEndState(GameState* game, Connection* conn, Player* player);
void ParseAssignPlayerId(GameState* game, Connection* conn, Player* player);
void SendReadyRequest(Player* player, Connection* conn);
void SendRestartRequest(GameState* game, Connection* conn, Player* player);
void UpdateClientPlayerCopies(std::array<Player, 4>& players, GameState* game);
void SendGameStateRequest(GameState* game, Connection* conn);
void RequestStateUpdate(GameState* game, Connection* conn, Player* player);
void DrawGameState(std::array<Player, 4> players);
void DrawLobbyState(GameState* game);