#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.h"
#include "Item.h"
#include "TileLayer.h"
#include "TilePaletteTile.h"

enum class ToolType { Room };

struct Tool {
    ToolType type;
    int sprite_map_index;
};

struct HouseSceneEditorState {
    bool editor_enabled = false;
    bool panning = false;
    int selected_tile_index = 0;
    // sf::IntRect tile_palette_bounds;
    sf::IntRect editor_selection;
    int selected_tile_layer;
    int selected_tile_rotation = 0;
    // int tile_palette_background_total_height;
    // sf::RectangleShape tile_palette_background;
    sf::RectangleShape tile_palette_selection_rectangle;
    // sf::View tile_palette_view;
    // sf::View toolbar_view;
    std::vector<TilePaletteTile> tile_palette_tiles;
    sf::Vector2i selected_editor_square;
    bool mouse_down = false;
    bool mouse_dragging = false;
    sf::Vector2f mouse_down_at;
    sf::Vector2f current_mouse_position;
    sf::IntRect map_selection;
    std::vector<Tool> tools;
    // int toolbar_icon_size = 15;
    // int toolbar_icon_padding = 10;
    // int left_toolbar_width;
};

struct HouseSceneState {
    std::vector<Entity> entities;
    std::vector<TileLayer> tile_layers;
    std::vector<Item> player_items;
    // sf::IntRect map_bounds;
    // sf::View house_view;
    HouseSceneEditorState editor_state;
    // int window_width;
    // int window_height;
    // int scale = 4;
    std::vector<sf::IntRect> rooms;
};
