#pragma once
#include <s_protocol.hpp>
#include <server_logic.hpp>
#include <s_player.hpp>


void ParseGameStateRequest(std::array<uint8_t, 28>& current_game_state, std::array<uint8_t, 32>& last_recieved_bytes, GameState& game_state, ServerStage& stage);
void ParsePlayerReadyRequest(std::array<uint8_t, 32>& message);
void UpdateGameState(std::array<uint8_t, 32>& message, ServerStage& stage);
void ChangeGameState();
void UpdateGameStateWithoutRequest();
void UpdateLobbyState(std::array<uint8_t, 32>& message);
void BroadcastStageData();
void InitGameState(GameState* game);
void LoadStageData(std::array<uint8_t, 32>& message);
void ParseSerialStageData(std::array<uint8_t, 32>& message, ServerStage& stage);
void ProcessPlayerFC();
void ProcessPlayerAttacks(float scale);
void ProcessPlayerPhysics();
Rectangle GeneratePlayerHitboxKick(Rectangle rect, float scale, int face);
Rectangle GeneratePlayerHitboxPunch(Rectangle rect, int face);