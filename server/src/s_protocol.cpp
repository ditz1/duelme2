#include "../inc/s_protocol.hpp"

bool game_running;
std::array<bool, 4> player_ready = {false, false, false, false};
std::vector<std::shared_ptr<websocket::stream<tcp::socket>>> clients;
std::mutex clients_mutex; // Mutex to protect the clients vector
int num_connections = -1;
int current_game_stage = 0;
GameState game_state;