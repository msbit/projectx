#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "SpriteSheet.h"

struct Tile {
    int x;
    int y;
    int rotation;
    int tile_map_index;
};

struct TileLayer {
    int layer_index;
    std::vector<Tile> tiles;
};

// A map holds all data for a given tile layout within the game.
class Map {
    int map_file_version;
    sf::IntRect bounds;
    std::vector<TileLayer> tile_layers;
    std::vector<Entity> entity;

public:
	Map(std::string);
	Map(int, int, int, int);

	void WriteToFile(std::string) const;
    sf::IntRect GetBounds();
    std::vector<TileLayer>& GetTileLayers();
    void AddTile(int, Tile);
};

