#pragma once
#include "../inc/s_protocol.hpp"

void ParseMessageReceived(std::array<uint8_t, 32>& message);
void SendToClient(std::shared_ptr<websocket::stream<tcp::socket>> client, std::array<uint8_t, 32> message);
void BroadcastMessage(std::array<uint8_t, 32>& message);
void ReAssignPlayerIds();
void SendBackPlayerId(size_t client_id);
void LogMessageReceived(std::array<uint8_t, 32>& message);
