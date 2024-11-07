#pragma once
#include <globals.hpp>
#include <player.hpp>
#include <helpers.hpp>
#include <networking.hpp>
#include <stage_manager.hpp>
#include <player_anims.hpp>

extern Stage stage;

void InitGameState(GameState* game);
void ParseGameState(GameState* game, Connection* conn, Player* player);
int ParseLobbyState(GameState* game, std::array<Player, 4>& players);
void LoadGameState(GameState* game, Player& client, std::array<Player, 4>& players);
void StartSendStageData(Connection* conn, std::array<Player, 4>& players, Stage& stage);
void ListenStageData(Connection* conn, Player& client, std::array<Player, 4>& players, Stage& stage);
void EndSendStageData(Connection* conn, std::array<Player, 4>& players, Stage& stage);
void SendStageData(Connection* conn, Player& client, std::array<Player, 4>& players, Stage& stage);
std::vector<uint8_t> SerializeStageData(std::vector<Rectangle>& stage_cells, std::vector<Rectangle>& player_cells);
std::vector<std::array<uint8_t, 32>> CreateStageMessage(std::vector<uint8_t> serialized_data);
void ParseEndState(GameState* game, Connection* conn, Player* player);
void ParseAssignPlayerId(GameState* game, Connection* conn, Player* player);
void SendReadyRequest(Player* player, Connection* conn);
void SendRestartRequest(GameState* game, Connection* conn, Player* player);
void UpdateClientPlayerCopies(std::array<Player, 4>& players, GameState* game);
void AdjustPlayerDimensions(Player& client, std::array<Player, 4>& all_players);
void SendGameStateRequest(GameState* game, Connection* conn);
void RequestStateUpdate(GameState* game, Connection* conn, Player* player);
void DrawGameState(std::array<Player, 4> players);
void DrawLobbyState(GameState* game);