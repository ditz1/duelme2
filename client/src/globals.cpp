#include <globals.hpp>

int this_client_id;
int num_players_connected = 0;
bool developer_mode = false;

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

void HandleErrors(Connection& conn){
    if (num_failed_pings > 10) {
        std::cout << "Connection lost" << std::endl;
        conn.connected = false;
        exit(1);
    }
}

std::tuple<uint8_t, uint8_t> Float16ToBytes(float f) {
    uint16_t bits = (uint16_t)f;
    union {
        uint16_t b;
        struct as_bits {
            uint8_t b2;
            uint8_t b1;
        } as_bits;
    } u;
    u.b = bits;
    uint8_t b1 = u.as_bits.b1;
    uint8_t b2 = u.as_bits.b2;
    return std::make_tuple(b1, b2);
}