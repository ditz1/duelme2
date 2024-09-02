#pragma once
#include "globals.hpp"
#include <string>

void OpenWebSocket(Connection* conn, const char* url);
void CloseWebSocket(EMSCRIPTEN_WEBSOCKET_T ws);
void ClientSendBytes(EMSCRIPTEN_WEBSOCKET_T ws, void* data, uint32_t length);
EM_BOOL OnMessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData);
EM_BOOL OnOpen(int eventType, const EmscriptenWebSocketOpenEvent* websocketEvent, void* userData);

// Implement these functions in networking.cpp