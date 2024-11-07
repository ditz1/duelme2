#include "networking.hpp"
#include <iostream>

void OpenWebSocket(Connection* conn, const char* url) {    
    if (!emscripten_websocket_is_supported()) {
        std::cout << "WebSockets are not supported" << std::endl;
        return;
    }
    
    EmscriptenWebSocketCreateAttributes ws_attrs = {
        url, nullptr, EM_TRUE
    };

    conn->ws = emscripten_websocket_new(&ws_attrs);
    if (conn->ws <= 0) {
        std::cout << "Failed to create WebSocket" << std::endl;
    } else {
        conn->connected = false;  // Will be set to true in OnOpen callback
        emscripten_websocket_set_onopen_callback(conn->ws, conn, OnOpen);
        emscripten_websocket_set_onmessage_callback(conn->ws, conn, OnMessage);
    }
}

void CloseWebSocket(Connection* conn) {
    emscripten_websocket_close(conn->ws, 1000, "Closed");
    emscripten_websocket_delete(conn->ws);
}

void ClientSendBytes(Connection* conn, void* data, uint32_t length) {
    EM_BOOL error = emscripten_websocket_send_binary(conn->ws, data, length);
    if (error) {
        std::cout << "Failed to send binary data" << std::endl;
    }
}

EM_BOOL OnMessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData) {
    Connection* conn = (Connection*)userData;
    if (websocketEvent->isText) {
        std::cout << "ERROR: server sent data as text instead of binary" << std::endl;
    } else {
        const uint8_t* binaryData = websocketEvent->data;
        size_t dataLength = websocketEvent->numBytes;        
        if (developer_mode) std::cout << "Received binary message of " << dataLength << " bytes" << std::endl;
        data_from_server.clear();
        switch(binaryData[0]){
            case msg_connect:
            case msg_disconnect:
            case msg_assign_id:
            case msg_update:
            case msg_lobby:
            case msg_stage_data:
                for (int i = 0; i < dataLength; i++) {
                    data_from_server.push_back(binaryData[i]);
                    if (developer_mode) printf("%x | ", binaryData[i]);
                }
                break;
            default:
                std::cout << "Recieved Malformed Data" << std::endl;
                break;
        }
         
    }
    conn->last_received = (EmscriptenWebSocketMessageEvent*)websocketEvent;
    return EM_TRUE;
}

EM_BOOL OnOpen(int eventType, const EmscriptenWebSocketOpenEvent* websocketEvent, void* userData) {
    std::cout << "WebSocket connection opened" << std::endl;
    Connection* conn = (Connection*)userData;
    conn->connected = true;
    return EM_TRUE;
}