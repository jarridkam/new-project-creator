#pragma once
#include <string.h>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <raylib.h>

using json = nlohmann::json;

namespace world{
    struct Tileset {
        std::string image_source;
        std::string name;

        int columns;
        int tile_width;
        int tile_height;
        int image_width;
        int image_height;
        int tile_count;
        int margin;
        int spacing;

        Texture2D texture{};

        std::vector<Rectangle> tile_rectangles;
    };

    struct Layer{

        uint8_t id;

        std::string  name;

        int     width;
        int     height;
        int     x;
        int     y;
        float   opacity;

        bool    visible;

        std::string type;
        std::vector<int> data;
    };


    struct Tilemap{
        std::string tileset_source;
        Tileset     tileset;
        int         tileset_first_global_id;

        std::vector<Layer> layers;

        int height;
        int width;
        int tile_width;
        int tile_height;
    };

    Tileset  LoadTileset(std::string file_path);
    Tilemap* LoadTileMap(std::string file_path);
    void     DrawTilemap(const Tilemap& map);
    void     UnloadTileMap(Tilemap* map);
}
