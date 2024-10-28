#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <game_manager.hpp>
#include <helpers.hpp>

int current_game_stage = 0;

int main() {
    
    InitWindow(800, 450, "client");
    SetTargetFPS(60);
    
    GameState game_state;


    InitGameState(&game_state);

    Player client_player;

    std::cout << "test" << std::endl;


    std::array<Player, 4> all_players = {Player(), Player(), Player(), Player()};
    for (int i = 0; i < 4; i++) {
        all_players[i].SetId(i);
    }
    Connection conn;
    OpenWebSocket(&conn, "ws://192.168.1.42:9000/ws");

    
    if (conn.ws <= 0) {
        std::cout << "Failed to create websocket" << std::endl;
        return 1;
    }

    while (!conn.connected) {
        emscripten_sleep(10); 
    }

    std::array<uint8_t, 32> buf;
    buf[0] = msg_connect;
    ClientSendBytes(&conn, (void*)buf.data(), 32);

    client_player.SetTexture(1);

    buf[0] = msg_ping;
    buf[1] = 0x00;
    buf[2] = msg_end;

    std::cout << "Loading texture" << std::endl;

    if (client_player.Id() > 0 && client_player.Id() < 4) {
        all_players[client_player.Id()] = client_player;
    } 

    // eventually will need something for managing the texture assignment
    // all_players[client_player.Id()].SetTexture(1);

    while (!WindowShouldClose()) {
        // todo: really we should just move the client_player into the all_players array
        // but i dont want to rewrite the draw function since even though it would be better to have pointers
        // so do this for now but this is still bad

        // born to do id forced to do Id
        all_players[client_player.Id()] = client_player;
        all_players[client_player.Id()].anim_current_frame = client_player.anim_current_frame;
        all_players[client_player.Id()].SetState(client_player.State());
        all_players[client_player.Id()].SetIsAnimating(client_player.IsAnimating());


        switch (current_game_stage){
            case 0:
                ParseAssignPlayerId(&game_state, &conn, &client_player);
                if (IsKeyPressed(KEY_SPACE)){
                    SendReadyRequest(&client_player, &conn);
                }
                ParseLobbyState(&game_state, all_players);
                break;
            case 1:
                UpdateClientPlayerCopies(all_players, &game_state);
                ParseGameState(&game_state, &conn, &client_player);
                break;
            case 2:
                ParseEndState(&game_state, &conn, &client_player);
                break;

        }


        if (IsKeyPressed(KEY_L)){            
            LogGameState(game_state, &conn);
        }
        if (IsKeyPressed(KEY_P)){
            std::cout << "ping" << std::endl;
            ClientSendBytes(&conn, (void*)buf.begin(), 32);
        }
        client_player.PollInput();
        RequestStateUpdate(&game_state, &conn, &client_player);        
        BeginDrawing();
            ClearBackground(DARKGRAY);
            //client_player.Draw();
            switch(current_game_stage){
                case 0:
                    DrawLobbyState(&game_state);
                    break;
                case 1:
                    DrawGameState(all_players);
                    break;
                case 2:
                    DrawText("Game Over", 400, 225, 20, RED);
                    break;
            }
            DrawDebugInfo(game_state, client_player, all_players);
            
        EndDrawing();

    }

    buf[0] = msg_disconnect;
    ClientSendBytes(&conn, (void*)&buf, 32);


    std::cout << "Closing application" << std::endl;

    //UnloadTexture(client_player.tex);
    //UnloadImage(client_player.img);

    CloseWebSocket(&conn);
    CloseWindow();

    return 0;
}