#include <globals.hpp>
#include <networking.hpp>
#include <player.hpp>
#include <item.hpp>
#include <game_manager.hpp>
#include <helpers.hpp>
#include <stage_manager.hpp>
#include <camera_manager.hpp>

int current_game_stage = 0;
int num_failed_pings = 0;
bool stage_sent = true;
bool in_loading_screen = false;
bool in_reset_screen = false;
bool stage_message_created = false;
bool debug_mode = false;
float player_size = 1.45;
int max_camera_y = 0;
int check_adjust = 0;
bool player_size_set = false;
bool single_player_mode = false;
bool can_move_bot = true;
bool can_move_player = true;
Vector2 center_stage = {0, 0};

int round_number = 1;
std::array<int, 4> player_wins = {0, 0, 0, 0};

float dt = 0.016f;

std::vector<std::string> stages;
int current_stage = 0;
bool loaded_textures = false;




Stage stage;

std::string test = "RRRRRR \
                    RRGGRR \
                    RRRRRR ";

// 7 x 9              0 1 2 3 4 5 6 7 8 9101112
std::string test2 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

std::string test3 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  G  G  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  #  #  #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  #  #  #  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  R  R  R  R  R  R  R  #  #  #  #  #  #  #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

std::string test4 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  G  G  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  #  #  #  #  #  #  #  #  #  #  #  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  #  #  #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  #  #  #  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

std::string test5 = "\r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \r \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  G  G  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  #  #  #  #  #  #  #  #  #  #  #  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  #  #  #  R  R  R  R  R  R  R  # \n \
                      #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  R  # \n \
                      #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # \n ";

void InLoadingScreen(){
    BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Loading...", 400, 225, 20, BLUE);
        DrawText(TextFormat("current game stage %d", current_game_stage), 400, 250, 20, BLUE);
    EndDrawing();
}

void CheckPlayersScale(Player& client, std::array<Player, 4>& players, Stage& stage){
    if (player_size_set && current_game_stage == 1) return;
    if (stage.cell_size <= client.Bounds().width) {
        AdjustPlayerDimensions(client, players);
    }
    for (auto& player : players){
        if (stage.cell_size <= player.Bounds().width) {
            AdjustPlayerDimensions(client, players);
        }
    }
    int lowest_width = 10000;
    int lowest_height = 10000;
    for (auto& player : players){
        if (player.draw_data.dest.width < lowest_width) {
            lowest_width = player.draw_data.dest.width;
            lowest_height = player.draw_data.dest.height;
        }
    }
    for (auto& player : players){
        player.draw_data.dest.width = lowest_width;
        player.draw_data.dest.height = lowest_height;
    }
    player_size_set = true;
}

int main() {
    std::string server_ip = "";
    std::string port = "9000";
#ifdef LOCAL
    server_ip = "127.0.0.1";
#else
    server_ip = "192.168.1.42";
#endif
    
    InitWindow(800, 450, "client");
    SetTargetFPS(60);

    stages.push_back(test3);
    stages.push_back(test4);
    stages.push_back(test5);

    
    GameState game_state;

    InitGameState(&game_state);

    Player client_player;

    std::array<Player, 4> all_players = {Player(), Player(), Player(), Player()};

    stage.tileset = LoadTexture("assets/tiles.png");

    stage.LoadFromString(test3);
    stage.Generate();
    std::vector<Rectangle> items_rec;

    std::string url = "ws://" + server_ip + ":" + port + "/ws";
    Connection conn;
    OpenWebSocket(&conn, url.c_str());

    Camera2D camera = { 0 };
    camera.target = { 0, 0 };
    camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 0.95f;

   

    max_camera_y = stage.cell_size * stage.rows;
    Vector2 max_cam_pos = GetScreenToWorld2D({0, (float)max_camera_y + 50.0f}, camera);
    max_camera_y = max_cam_pos.y;

    
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

    std::vector<Item> items;
    Item item;
    item.type = SHOTGUN;
    item.player_assigned = 0;
    std::string shotgun_path = "assets/weapons/shotgun.png";
    item.LoadTextures(shotgun_path);

    Item item2;
    item2.type = SHOTGUN;
    item2.player_assigned = 1;
    item2.LoadTextures(shotgun_path);

    items.push_back(item);
    items.push_back(item2);

    Player dummy_player;
    // this isn't exactly the right way of doing this,
    // because really we should only be using the textures
    // for each player in the game, so now if there are only 2
    // we still load 4 textures - however, if we don't do this,
    // then the client will have its id malformed on page refresh
    // and textures wont load

    client_player.SetId(0);
    dummy_player.SetId(1);
    all_players[0].SetId(0);
    all_players[1].SetId(1);
    all_players[2].SetId(2);
    all_players[3].SetId(3);

    // eventually will need something for managing the texture assignment
    int lobby_state = 0;
    float bot_ping_timer = 0;

    while (!WindowShouldClose()) {
        // todo: really we should just move the client_player into the all_players array
        // but i dont want to rewrite the draw function since even though it would be better to have pointers
        // so do this for now but this is still bad
        max_camera_y = stage.cell_size * stage.rows;
        Vector2 max_cam_pos = GetScreenToWorld2D({0, (float)max_camera_y + 50.0f}, camera);
        max_camera_y = max_cam_pos.y;
        
        all_players[this_client_id] = client_player;
        all_players[this_client_id].SetId(this_client_id);
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
            SendStageData(&conn, client_player, all_players, stage, items_rec);
            ListenStageData(&conn, client_player, all_players, stage);
            center_stage = { (float)stage.cols * stage.cell_size / 2, (float)stage.rows * stage.cell_size / 2 };
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
                        SendStageData(&conn, client_player, all_players, stage, items_rec);
                        LoadGameState(&game_state, client_player, all_players);
                        if (num_players_connected < 1){
                            std::cout << "single player mode" << std::endl;
                            single_player_mode = true;
                        }
                        break;
                }
                if (!loaded_textures){
                    for (auto& player : all_players){
                        player.LoadTextures();
                        std::cout << "loaded textures for player " << player.Id() << std::endl;
                    }
                    loaded_textures = true;
                }
                //UpdateClientPlayerCopies(all_players, &game_state);
                UpdateItems(all_players, items);
                break;
            case 1:
                if (check_adjust < 3) {
                    AdjustPlayerDimensions(client_player, all_players);
                    check_adjust++;
                }
                UpdateClientPlayerCopies(all_players, &game_state);
                UpdateItems(all_players, items);
                ParseGameState(&game_state, &conn, &client_player);
                break;
            case 2:
                can_move_bot = false;
                can_move_player = false;
                ParseEndState(&game_state, &conn, &client_player);
                UpdateClientPlayerCopies(all_players, &game_state);
                UpdateItems(all_players, items);
                ParseGameState(&game_state, &conn, &client_player);
                if (reset_timer <= 0.0f) {
                    
                    current_game_stage = 1;
                    round_number++;
                    can_move_bot = true;
                    can_move_player = true;
                }
                break;
        }



        if (IsKeyPressed(KEY_L)){            
            LogGameState(game_state, &conn);
        }
        
        if (IsKeyPressed(KEY_P)){
            std::cout << "ping" << std::endl;
            ClientSendBytes(&conn, (void*)buf.begin(), 32);
        }



        if (IsKeyPressed(KEY_ENTER)){
            debug_mode = !debug_mode;
        }

        if (can_move_player) client_player.PollInput();

        if (IsKeyPressed(KEY_W)){
           client_player.SetRequestedState(MOVE_UP);
        }

        RequestStateUpdate(&game_state, &conn, &client_player);   
        all_players[this_client_id].SetPosition(client_player.Position());
        for (Player& p : all_players){ 
            p.Update(); 
        }
        all_players[this_client_id].SetFaceDir(client_player.FaceDir());

        if (single_player_mode && game_state.player_states[1] != MOVE_DOWN){
            bot_ping_timer += dt;
            // so there needs to be a request every frame so that the server can update the 
            // bot at the same frequency, else the bot will move slower - but the cost
            // is now the client does the work of 2
            if (bot_ping_timer >= dt){
                RequestDummyStateUpdate(&game_state, &conn, &dummy_player);
                bot_ping_timer = 0;
            }
        }
        
        AdjustCameraPosition(all_players, camera, max_camera_y, center_stage);
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
                        DrawGameState(all_players, items);
                    EndMode2D();
                    DrawGameUI(game_state, client_player, all_players);
                    break;
                case 2:
                    BeginMode2D(camera);
                        stage.Draw();
                        DrawGameState(all_players, items);
                    EndMode2D();
                    DrawText("Game Over", 400, 225, 20, RED);
                    DrawText(TextFormat("reset in %.2f", reset_timer), 400, 250, 20, RED);
                    break;
            }
            
        
        if (debug_mode) DrawDebugInfo(game_state, client_player, all_players);
            
        EndDrawing();

    }

    buf[0] = msg_disconnect;
    ClientSendBytes(&conn, (void*)&buf, 32);
    // unload all textures
    for (auto& player : all_players){
        player.UnloadTextures();
    }

    client_player.UnloadTextures();

    for (auto& item : items){
        item.UnloadTextures();
    }
    


    std::cout << "Closing application" << std::endl;

    //UnloadTexture(client_player.tex);
    //UnloadImage(client_player.img);

    CloseWebSocket(&conn);
    CloseWindow();

    return 0;
}