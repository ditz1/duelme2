#include <globals.hpp>

int this_client_id;
int num_players_connected = 0;

std::vector<uint8_t> data_from_server;
std::array<bool, 4> player_ready = {false, false, false, false};



std::string PlayerStateToString(PlayerState state) {
    int state_int = static_cast<int>(state);
    if (state_int >= 0 && state_int < ALL_PLAYER_STATES) {
        return PlayerStateToStringMap.at(state);
    }
    return "INVALID_STATE";
}


Vector2 V2intToV2(Vector2int v) {
    int x = int(v.x);
    int y = int(v.y);
    return Vector2{float(x), float(y)};
}