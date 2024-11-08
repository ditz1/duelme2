#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <game_manager.hpp>
#include <helpers.hpp>
#include <stage_manager.hpp>
#include <camera_manager.hpp>

int current_game_stage = 0;
int num_failed_pings = 0;
bool stage_sent = true;
bool in_loading_screen = false;
bool stage_message_created = false;

Stage stage;

std::string test = "RRRRRR \
                    RRGGRR \
                    RRRRRR ";

// 7 x 9              0 1 2 3 4 5 6 7 8 9101112
std::string test2 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  G  G  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

std::string test3 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  G  G  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  #  #  #  #  #  #  #  #  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  #  #  #  #  R  R  R  R  R  R  R  R  R  #  #  #  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

std::string test4 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

void InLoadingScreen(){
    BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Loading...", 400, 225, 20, BLUE);
        DrawText(TextFormat("current game stage %d", current_game_stage), 400, 250, 20, BLUE);
    EndDrawing();
}

void CheckPlayersScale(Player& client, std::array<Player, 4>& players, Stage& stage){
    if (stage.cell_size <= client.Bounds().width) {
        AdjustPlayerDimensions(client, players);
    }
    for (auto& player : players){
        if (stage.cell_size <= player.Bounds().width) {
            AdjustPlayerDimensions(client, players);
        }
    }
}

int main() {
    
    InitWindow(800, 450, "client");
    SetTargetFPS(60);
    
    GameState game_state;

    InitGameState(&game_state);

    Player client_player;

    std::array<Player, 4> all_players = {Player(), Player(), Player(), Player()};

    stage.LoadFromString(test4);
    stage.Generate();

    Connection conn;
    OpenWebSocket(&conn, "ws://192.168.1.42:9000/ws");

    Camera2D camera = { 0 };
    camera.target = { 0, 0 };
    camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    
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

    buf[0] = msg_ping;
    buf[1] = 0x00;
    buf[2] = msg_end;

    std::cout << "Loading texture" << std::endl;

    for (int i = 0; i < 4; i++) {
        if (i == client_player.Id()) continue;
        all_players[i].SetId(i);
    }
    
    this_client_id = client_player.Id();

    // while (client_player.Bounds().width >= stage.cell_size) {
    //     client_player.draw_data.scale -= 0.1f;
    // }

    // eventually will need something for managing the texture assignment
    int lobby_state = 0;

    while (!WindowShouldClose()) {
        // todo: really we should just move the client_player into the all_players array
        // but i dont want to rewrite the draw function since even though it would be better to have pointers
        // so do this for now but this is still bad
        
        // born to do id forced to do Id
        all_players[client_player.Id()] = client_player;
        all_players[client_player.Id()].anim_current_frame = client_player.anim_current_frame;
        all_players[client_player.Id()].SetState(client_player.State());
        all_players[client_player.Id()].SetIsAnimating(client_player.IsAnimating());

        if (IsKeyDown(KEY_LEFT)) camera.target.x -= 10;
        if (IsKeyDown(KEY_RIGHT)) camera.target.x += 10;
        if (IsKeyDown(KEY_UP)) camera.target.y -= 10;
        if (IsKeyDown(KEY_DOWN)) camera.target.y += 10;
        if (IsKeyDown(KEY_COMMA)) camera.zoom += 0.01f;
        if (IsKeyDown(KEY_PERIOD)) camera.zoom -= 0.01f;

        CheckPlayersScale(client_player, all_players, stage);

        if (in_loading_screen) {
            InLoadingScreen();
            SendStageData(&conn, client_player, all_players, stage);
            ListenStageData(&conn, client_player, all_players, stage);
            continue;
        }
       
        switch (current_game_stage){
            case 0:
                ParseAssignPlayerId(&game_state, &conn, &client_player);
                if (IsKeyPressed(KEY_SPACE)){
                    SendReadyRequest(&client_player, &conn);
                }
                lobby_state = ParseLobbyState(&game_state, all_players);
                switch(lobby_state){
                    case 0:
                        break;
                    case 1:
                        SendStageData(&conn, client_player, all_players, stage);
                        LoadGameState(&game_state, client_player, all_players);
                        break;
                }
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

        
        for (Player& p : all_players){ 
            p.Update(); 
        }

        client_player.PollInput();
        RequestStateUpdate(&game_state, &conn, &client_player);   
        
        AdjustCameraPosition(all_players, camera);
        /////// draw /////////
        BeginDrawing();
            ClearBackground(DARKGRAY);

            switch(current_game_stage){
                case 0:
                    DrawLobbyState(&game_state);
                    DrawText(TextFormat("lobby state: %d", lobby_state), 400, 225, 20, BLUE);
                    break;
                case 1:
                    BeginMode2D(camera);
                        stage.Draw();
                        stage.DrawLines();
                        DrawGameState(all_players);
                    EndMode2D();

                    break;
                case 2:
                    DrawText("Game Over", 400, 225, 20, RED);
                    break;
            }
            DrawDebugInfo(game_state, client_player, all_players);
            
        EndDrawing();

        HandleErrors(conn);
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