#pragma once
#include "globals.hpp"
#include <string>

void OpenWebSocket(Connection* conn, const char* url);
void CloseWebSocket(Connection* conn);
void ClientSendBytes(Connection* conn, void* data, uint32_t length);
EM_BOOL OnMessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData);
EM_BOOL OnOpen(int eventType, const EmscriptenWebSocketOpenEvent* websocketEvent, void* userData);
