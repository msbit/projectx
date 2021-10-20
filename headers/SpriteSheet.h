#pragma once

#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Screen.h"

class SpriteSheet {
    std::string texture_path;
    sf::Sprite CreateTileSprite(int, int);
    std::vector<sf::Sprite> sprites;
    const int frame_tile_size;
    sf::Texture texture;
    sf::RenderTexture icon_sprite_render_texture;
    void LoadFromFile();
    std::shared_ptr<Screen> screen;

  public:
    SpriteSheet(std::string, int, int, int, std::shared_ptr<Screen>);
    SpriteSheet(int, std::shared_ptr<Screen>);

    // TODO : Not sure if this should be here. We might need another time of
    // sprite sheet to handle this.
    void CreateIconSprite(sf::Color, int);

    int GetSpriteSize() const;
    const std::vector<sf::Sprite> &GetSprites() const;
    const sf::Texture &GetTexture() const;
    int GetFrameTileSize() const;
    void Reload();
};
