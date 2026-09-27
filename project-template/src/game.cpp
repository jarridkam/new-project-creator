#include "game.h"

namespace template_game{

    static world::Tilemap* loaded_tilemap = nullptr;
    static Camera2D camera = { 0 };

    void Update(){
        if (IsKeyDown(KEY_RIGHT)) camera.target.x += 4;
        if (IsKeyDown(KEY_LEFT))  camera.target.x -= 4;
        if (IsKeyDown(KEY_DOWN))  camera.target.y += 4;
        if (IsKeyDown(KEY_UP))    camera.target.y -= 4;

        camera.zoom += GetMouseWheelMove() * 0.05f;
        if (camera.zoom < 0.1f) camera.zoom = 0.1f;
    }

    void Render(){
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
                if (loaded_tilemap) world::DrawTilemap(*loaded_tilemap);
            EndMode2D();
            DrawText("TEMPLATE GAME", 16, 16, 24, WHITE);
            DrawText("Arrows: move camera   Wheel: zoom", 16, 48, 16, LIGHTGRAY);
            DrawFPS(16, 72);
        EndDrawing();
    }

    void Initialize(){

        const int screenWidth = 800;
        const int screenHeight = 450;

        InitWindow(screenWidth, screenHeight, "TEMPLATE GAME");

        SetTargetFPS(60);

        camera.target = { 0.0f, 0.0f };
        camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        std::string assets_path = std::string(GetApplicationDirectory()) + "../assets/";
        loaded_tilemap = world::LoadTileMap(assets_path + "tilemaps/test.json");
    }

    void Run(){
        while (!WindowShouldClose())
        {
            Update();
            Render();
        }
    }

    void Shutdown(){
        world::UnloadTileMap(loaded_tilemap);
        loaded_tilemap = nullptr;

        CloseWindow();
    }
}
