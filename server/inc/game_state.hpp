#pragma once
#include <s_protocol.hpp>
#include <server_logic.hpp>
#include <s_player.hpp>
#include <hitboxes.hpp>

extern std::array<PlayerBody, 4> player_bodies;

void ParseGameStateRequest(std::array<uint8_t, 28>& current_game_state, std::array<uint8_t, 32>& last_recieved_bytes, GameState& game_state, ServerStage& stage);
void ParsePlayerReadyRequest(std::array<uint8_t, 32>& message);
void UpdateGameState(std::array<uint8_t, 32>& message, ServerStage& stage);
void ChangeGameState(bool restart = false);
void UpdateGameStateWithoutRequest();
void UpdateLobbyState(std::array<uint8_t, 32>& message);
void BroadcastStageData();
void InitGameState(GameState* game);
void LoadStageData(std::array<uint8_t, 32>& message);
void ParseSerialStageData(std::array<uint8_t, 32>& message, ServerStage& stage);
void ProcessPlayerFC();
void ProcessPlayerAttacks(float scale);
void ProcessPlayerPhysics();
