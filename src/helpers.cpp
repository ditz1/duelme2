#include <helpers.hpp>

void DrawDebugInfo(GameState game) {
            int font_size = 20;
            int y_spacing = 30;
            int x_start = 30;
            int y_start = 30;
            DrawText("Client", x_start, y_start, font_size, RAYWHITE);
            DrawText("- Player 1 -", x_start, y_start + y_spacing, font_size, RAYWHITE);
            DrawText(TextFormat("State: %s", PlayerStateToString(PlayerState(game.player_states[0])).c_str()), x_start, y_start + y_spacing * 2, font_size, RAYWHITE);
}

void LogGameState(GameState game) {
    std::cout << "Player 1" << std::endl;
    std::cout << "state: " << PlayerStateToString(PlayerState(game.player_states[0])) << std::endl;
    std::cout << "pos: " << game.player_positions[0].x << "," << game.player_positions[0].y << std::endl;

    std::cout << "Player 2 state: " << std::endl; 
    std::cout << "state: " << PlayerStateToString(PlayerState(game.player_states[1])) << std::endl;
    std::cout << "pos: " << game.player_positions[1].x << "," << game.player_positions[1].y << std::endl;

    std::cout << "Player 3 state: " << std::endl;
    std::cout << "state: " << PlayerStateToString(PlayerState(game.player_states[2])) << std::endl;
    std::cout << "pos: " << game.player_positions[2].x << "," << game.player_positions[2].y << std::endl;

    std::cout << "Player 4 state: " << std::endl;
    std::cout << "state: " << PlayerStateToString(PlayerState(game.player_states[3])) << std::endl;
    std::cout << "pos: " << game.player_positions[3].x << "," << game.player_positions[3].y << std::endl;
    
    printf("\n");
    printf("%x\n", game.ToBytes().c_str());

}