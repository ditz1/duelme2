#include <helpers.hpp>
#include <player.hpp>

void DrawDebugInfo(GameState game, Player& client_player, std::array<Player, 4> all_players) {
            int font_size = 18;
            int y_spacing = 25;
            int x_start = 30;
            int y_start = 30;
            int y_start_other = 100;
            int x_start_other = GetScreenWidth() - 120;
            unsigned int frame_gap = 0;

            // right side
            DrawFPS(x_start_other, y_start_other - y_spacing);            
            DrawText(TextFormat("Stage: %d", current_game_stage), x_start_other, y_start_other, font_size, RAYWHITE);
            DrawText(TextFormat("Other Players Frame Data"), x_start_other, y_start_other + y_spacing, font_size, RAYWHITE);
            DrawText(TextFormat("Player 0: %d", all_players[0].anim_current_frame), x_start_other, y_start_other + y_spacing * 2, font_size, RAYWHITE);
            DrawText(TextFormat("Player 1: %d", all_players[1].anim_current_frame), x_start_other, y_start_other + y_spacing * 3, font_size, RAYWHITE);
            DrawText(TextFormat("Player 2: %d", all_players[2].anim_current_frame), x_start_other, y_start_other + y_spacing * 4, font_size, RAYWHITE);
            DrawText(TextFormat("Player 3: %d", all_players[3].anim_current_frame), x_start_other, y_start_other + y_spacing * 5, font_size, RAYWHITE);
            
            frame_gap = std::abs(all_players[0].anim_current_frame - all_players[1].anim_current_frame);
            if (all_players[0].State() == all_players[1].State()) {
                DrawText(TextFormat("Frame Gap: %d", frame_gap), x_start_other, y_start_other + y_spacing * 6, font_size - 4, RED);
            } 


            // left side
            DrawText("Client", x_start, y_start, font_size, RAYWHITE);
            DrawText(TextFormat("- Player %d -", game.player_ids[this_client_id]), x_start, y_start + y_spacing, font_size, RAYWHITE);
            DrawText(TextFormat("State: %s, %s", PlayerStateToString(PlayerState(game.player_states[this_client_id])).c_str(), PlayerStateToString(client_player.State()).c_str()) , x_start, y_start + y_spacing * 2, font_size, RAYWHITE);
            DrawText(TextFormat("RequestedState: %s", PlayerStateToString(client_player.RequestedState()).c_str()) , x_start, y_start + y_spacing * 3, font_size, RAYWHITE);
            DrawText(TextFormat("CurrentAnimFrame: %d", client_player.texs[client_player.current_anim].fc), x_start, y_start + y_spacing * 4, font_size, RAYWHITE);
            DrawText(TextFormat("IsAnimating: %d", client_player.IsAnimating()), x_start, y_start + y_spacing * 5, font_size, RAYWHITE);
            DrawText(TextFormat("Current Anim FC: %d", client_player.anim_current_frame), x_start, y_start + y_spacing * 6, font_size, RAYWHITE);
            DrawText(TextFormat("FaceDir: %d", client_player.FaceDir()), x_start, y_start + y_spacing * 7, font_size, RAYWHITE);
}

void LogGameState(GameState game, Connection* conn) {
    printf("Game State: %d\n", int(sizeof(game.ToBytes())));
    std::array<uint8_t, 28> bytes = game.ToBytes();
    for (int i = 0; i < sizeof(game.ToBytes()); i++) {
        printf(" %x |", bytes[i]);
    }
    printf("\n");
}

void StripStageString(std::string* stage_str) {

    char reserved_chars[] = {'!',' ', '\n', '\r', '\t'};

    for (char c : reserved_chars) {
        for (int i = 0; i < stage_str->size(); i++) {
            if (stage_str->at(i) == c) {
                stage_str->erase(i, 1);
            } else {
                stage_str->erase(remove_if(stage_str->begin(), stage_str->end(), isspace));
            }
        }
    }
}
