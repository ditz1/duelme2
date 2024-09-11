#include <helpers.hpp>
#include <player.hpp>

void DrawDebugInfo(GameState game, Player& client_player) {
            int font_size = 20;
            int y_spacing = 30;
            int x_start = 30;
            int y_start = 30;
            DrawText("Client", x_start, y_start, font_size, RAYWHITE);
            DrawText(TextFormat("- Player %d -", game.player_ids[this_client_id]), x_start, y_start + y_spacing, font_size, RAYWHITE);
            DrawText(TextFormat("State: %s", PlayerStateToString(PlayerState(game.player_states[this_client_id])).c_str()), x_start, y_start + y_spacing * 2, font_size, RAYWHITE);
            DrawText(TextFormat("Position: %d, %d", game.player_positions[this_client_id].x, game.player_positions[this_client_id].y), x_start, y_start + y_spacing * 3, font_size, RAYWHITE);
}

void LogGameState(GameState game, Connection* conn) {
    std::cout << "recieved from server" <<  std::endl;
    printf("Game State: %d\n", int(sizeof(game.ToBytes())));
    for (int i = 0; i < sizeof(game.ToBytes()); i++) {
        printf(" %x |", (game.ToBytes())[i]);
    }
    printf("\n");
    for (int i = 0; i < conn->last_received->numBytes; i++) {
        printf("%x | ", conn->last_received->data[i]);
    }
    printf("\n");
}
