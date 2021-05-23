#include <fstream>
#include "Map.h"

Map::Map(std::string file_name) {
    std::ifstream rf(file_name, std::ios::in | std::ios::binary);

    if (!rf) {
        std::cout << "Can't read file!" << std::endl;
        exit(1);
    }
   
    rf.read(reinterpret_cast<char *>(&bounds.left), sizeof (bounds.left));
    rf.read(reinterpret_cast<char *>(&bounds.top), sizeof (bounds.top));
    rf.read(reinterpret_cast<char *>(&bounds.width), sizeof (bounds.width));
    rf.read(reinterpret_cast<char *>(&bounds.height), sizeof (bounds.height));

    int layer_count;
    rf.read(
        reinterpret_cast<char *>(&layer_count), 
        sizeof(layer_count)
    );

    for(auto i = 0; i < layer_count; i++) {
        int room_tile_count;
        rf.read(
            reinterpret_cast<char *>(&room_tile_count), 
            sizeof(room_tile_count)
        );

        std::vector<MapTile> tiles;

        for(auto i = 0; i < room_tile_count; i++) {
            MapTile tile;
            rf.read(
                reinterpret_cast<char *>(&tile.rotation), 
                sizeof (tile.rotation)
            );

            rf.read(
                reinterpret_cast<char *>(&tile.tile_map_index), 
                sizeof (tile.tile_map_index)
            );

            rf.read(
                reinterpret_cast<char *>(&tile.x), 
                sizeof (tile.x)
            );

            rf.read(
                reinterpret_cast<char *>(&tile.y), 
                sizeof (tile.y)
            );

            tiles.push_back(tile);
        }

        tile_layers.push_back(TileLayer{i, tiles});
    }

}

Map::Map(int map_height, int map_width, int window_width, int window_height)
    : bounds(0, 0, map_width, map_height) {

    tile_layers.push_back(TileLayer{0, std::vector<MapTile>()});
}

void Map::WriteToFile(std::string file_name) const {
    std::ofstream wf(file_name, std::ios::out | std::ios::binary);

    if(!wf) {
        std::cout << "Can't open file!" << std::endl;
        exit(1);
    }

    wf.write(
        reinterpret_cast<const char *>(&bounds.left), 
        sizeof (bounds.left)
    );

    wf.write(
        reinterpret_cast<const char *>(&bounds.top), 
        sizeof (bounds.top)
    );

    wf.write(
        reinterpret_cast<const char *>(&bounds.width), 
        sizeof (bounds.width)
    );

    wf.write(
        reinterpret_cast<const char *>(&bounds.height), 
        sizeof (bounds.height)
    );

    for (const auto &tile_layer: tile_layers) {
        int room_tile_count = tile_layer.tiles.size();
        wf.write(reinterpret_cast<const char *>(&room_tile_count), sizeof (room_tile_count));

        for(const auto &t: tile_layer.tiles) {
            wf.write(
                reinterpret_cast<const char *>(&t.rotation), 
                sizeof (t.rotation)
            );

            wf.write(
                reinterpret_cast<const char *>(&t.tile_map_index), 
                sizeof (t.tile_map_index)
            );

            wf.write(
                reinterpret_cast<const char *>(&t.x), 
                sizeof (t.x)
            );

            wf.write(
                reinterpret_cast<const char *>(&t.y), 
                sizeof (t.y)
            );
        }

    }

}

sf::IntRect Map::GetBounds() {
    return bounds;
}

std::vector<TileLayer>& Map::GetTileLayers() {
    return tile_layers;
}

void Map::AddTile(int index, MapTile tile) {
    auto found = std::find_if(
        tile_layers[index].tiles.begin(), 
        tile_layers[index].tiles.end(), 
        [tile](const auto &t) {
            return t.x == tile.x && t.y == tile.y;
        }
    );

    if (found != tile_layers[index].tiles.end()) {
        tile_layers[index].tiles.erase(found);
    }
    tile_layers[index].tiles.push_back(tile);
}

void Map::AddEntity(Entity entity) {
    entities.push_back(entity);
}


std::vector<Entity>& Map::GetEntities() {
    return entities;
}
