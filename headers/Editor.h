#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Room.h"

/**
 * An editor contains all data required to add, remove and edit tiles on a given map
 */
typedef struct {
    sf::RectangleShape* selection_rectangle;	

    // The background shape for the tile toolbar editor
    sf::RectangleShape* background;	

    // A list of vector of spirte tiles for rendering in the sidebar of the editor
    std::vector<sf::Sprite>* tiles;

    // The offset defines gaps between the tiles on the left hand tile editor
    int offset;

    TileMap* tile_map;

    // The currently selected tile index. This is the index for the tiles vector
    int selected_tile_index;

    // The view for the current tile palette that appears on the left side of the window
    sf::View* tile_palette_view;

    sf::RenderTexture* tile_palette_render_texture;

} Editor;

Editor* CreateEditor(TileMap &tile_map, int window_height, int window_width);

void DestructEditor(Editor& editor);

void DrawEditor(sf::RenderTarget& target, Editor& editor, const Room& room);

void UpdateEditor(Editor& editor, Room& room, const sf::Event& event);

sf::Vector2i GetTilePositionAt(Room& room, int x, int y, int tile_map_size);

void WriteRoomToFile(Room& room, std::string file_name);

Room* ReadRoomFromFile(std::string file_name);