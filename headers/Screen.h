#pragma once

#include <SFML/Graphics/Rect.hpp>

class Screen {
    sf::IntRect window_size;
    int scale;
    int toolbar_icon_padding;
    int toolbar_icon_size;
    int tile_palette_offset;
    int toolbar_offset;
    const int tile_map_sprite_size;

  public:
    Screen(sf::IntRect, int, int, int, int, int, const int);
    const sf::IntRect GetButtonArea() const;
    const sf::IntRect GetWindowSize() const;
    const sf::IntRect GetTilePaletteArea() const;
    const sf::IntRect GetToolbarArea() const;
    int GetToolbarOffset() const;
    int GetTilePaletteOffset() const;
    int GetScale() const;
    int GetToolbarIconPadding() const;
};
