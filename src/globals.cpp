#include <globals.hpp>

std::string PlayerStateToString(PlayerState state) {
    int state_int = static_cast<int>(state);
    if (state_int > 0 && state_int < ALL_PLAYER_STATES) {
        return PlayerStateToStringMap.at(state);
    }
    return "INVALID_STATE";
}