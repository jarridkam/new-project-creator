#include "tilemap.h"
#include <filesystem>

namespace world{

    void from_json(const json& data, Tileset& tileset)
    {
        tileset.columns = data.at("columns").get<int>();
        tileset.image_source = data.at("image").get<std::string>();
        tileset.image_width = data.at("imagewidth").get<int>();
        tileset.image_height = data.at("imageheight").get<int>();
        tileset.tile_width = data.at("tilewidth").get<int>();
        tileset.tile_height = data.at("tileheight").get<int>();
        tileset.tile_count = data.at("tilecount").get<int>();
        tileset.margin = data.at("margin").get<int>();
        tileset.spacing = data.at("spacing").get<int>();
        tileset.name = data.at("name").get<std::string>();
    }

    void from_json(const json& data, Layer& layer){
        layer.id = data.at("id").get<int>();
        layer.name = data.at("name").get<std::string>();

        layer.width = data.at("width").get<int>();
        layer.height = data.at("height").get<int>();
        layer.x = data.at("x").get<int>();
        layer.y = data.at("y").get<int>();

        layer.opacity = data.at("opacity").get<float>();
        layer.visible = data.at("visible").get<bool>();
        layer.type = data.at("type").get<std::string>();

        layer.data =
            data.at("data").get<std::vector<int>>();
    }


    void from_json(const json& data, Tilemap& map){
        map.layers      = data.at("layers").get<std::vector<Layer>>();
        map.width       = data.at("width").get<int>();
        map.height      = data.at("height").get<int>();
        map.tile_width  = data.at("tilewidth").get<int>();
        map.tile_height = data.at("tileheight").get<int>();

        const json& tileset_reference = data.at("tilesets").at(0);
        map.tileset_source          = tileset_reference.at("source").get<std::string>();
        map.tileset_first_global_id = tileset_reference.at("firstgid").get<int>();
    }

    Tileset LoadTileset(std::string file_path){
        std::ifstream file(file_path);
        if (!file) throw std::runtime_error("LoadTileset: could not open '" + file_path + "'");

        json data = json::parse(file);
        Tileset tileset = data.get<Tileset>();

        std::filesystem::path image_path =
            std::filesystem::path(file_path).parent_path() / tileset.image_source;
        tileset.texture = LoadTexture(image_path.string().c_str());

        tileset.tile_rectangles.reserve(tileset.tile_count);
        for (int tile_id = 0; tile_id < tileset.tile_count; tile_id++){
            float source_x = (float)((tile_id % tileset.columns) * tileset.tile_width);
            float source_y = (float)((tile_id / tileset.columns) * tileset.tile_height);
            tileset.tile_rectangles.push_back({
                source_x, source_y,
                (float)tileset.tile_width, (float)tileset.tile_height
            });
        }

        return tileset;
    }

    Tilemap* LoadTileMap(std::string file_path){
        std::ifstream file(file_path);
        if (!file) throw std::runtime_error("LoadTileMap: could not open '" + file_path + "'");

        json data = json::parse(file);
        Tilemap* map = new Tilemap(data.get<Tilemap>());

        std::filesystem::path tileset_path =
            std::filesystem::path(file_path).parent_path() / map->tileset_source;
        map->tileset = LoadTileset(tileset_path.string());

        return map;
    }

    void DrawTilemap(const Tilemap& map){
        for (const Layer& layer : map.layers){
            if (!layer.visible) continue;

            for (int y = 0; y < layer.height; y++){
                for (int x = 0; x < layer.width; x++){
                    int global_tile_id = layer.data[y * layer.width + x];
                    if (global_tile_id == 0) continue;

                    int tile_id = global_tile_id - map.tileset_first_global_id;
                    if (tile_id < 0 || tile_id >= (int)map.tileset.tile_rectangles.size()) continue;

                    const Rectangle& source = map.tileset.tile_rectangles[tile_id];
                    Vector2 destination = {
                        (float)((layer.x + x) * map.tile_width),
                        (float)((layer.y + y) * map.tile_height)
                    };

                    DrawTextureRec(map.tileset.texture, source, destination, WHITE);
                }
            }
        }
    }

    void UnloadTileMap(Tilemap* map){
        if (!map) return;
        UnloadTexture(map->tileset.texture);
        delete map;
    }
}
