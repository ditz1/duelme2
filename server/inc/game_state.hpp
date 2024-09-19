#pragma once
#include "s_protocol.hpp"
#include "server_logic.hpp"

void ParseGameStateRequest(std::array<uint8_t, 28>& current_game_state, std::array<uint8_t, 32>& last_recieved_bytes, GameState& game_state);
void ParsePlayerReadyRequest(std::array<uint8_t, 32>& message);
void UpdateGameState(std::array<uint8_t, 32>& message);
void ChangeGameState();
void UpdateGameStateWithoutRequest();
void UpdateLobbyState(std::array<uint8_t, 32>& message);
void InitGameState(GameState* game);
