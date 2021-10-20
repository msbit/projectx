#include <iostream>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "SpriteSheet.h"

// For now we will assume that this is a constructor for a sprite sheet from a
// texture file
SpriteSheet::SpriteSheet(std::string texture_path, int frame_tile_size,
                         int cols, int rows, std::shared_ptr<Screen> screen)
    : texture_path(texture_path), sprites(), frame_tile_size(frame_tile_size),
      texture(), screen(screen) {

    LoadFromFile();
    for (auto y = 0; y < cols; y++) {
        for (auto x = 0; x < rows; x++) {
            sprites.push_back(CreateTileSprite(x, y));
        }
    }
};

// For now we will assume that this is a entity sprite sheet drawn with code
SpriteSheet::SpriteSheet(int frame_tile_size, std::shared_ptr<Screen> screen)
    : sprites(), frame_tile_size(frame_tile_size), texture(), screen(screen) {
    // TODO : This will never allow us to create more than two sprites
    icon_sprite_render_texture.create(frame_tile_size * 2, frame_tile_size);
};

void SpriteSheet::CreateIconSprite(sf::Color color, int render_offset) {
    auto icon_rect =
        sf::RectangleShape(sf::Vector2f(frame_tile_size, frame_tile_size));
    icon_rect.setFillColor(color);
    icon_rect.setPosition(frame_tile_size * render_offset, 0);
    icon_sprite_render_texture.draw(icon_rect);
    icon_sprite_render_texture.display();

    auto sprite = sf::Sprite(icon_sprite_render_texture.getTexture(),
                             sf::IntRect(frame_tile_size * render_offset, 0,
                                         frame_tile_size, frame_tile_size));
    sprite.setScale(screen->GetScale(), screen->GetScale());
    sprites.push_back(sprite);
}

sf::Sprite SpriteSheet::CreateTileSprite(int tile_map_x, int tile_map_y) {
    sf::Sprite sprite(texture, sf::IntRect(frame_tile_size * tile_map_x,
                                           frame_tile_size * tile_map_y,
                                           frame_tile_size, frame_tile_size));

    sprite.setScale(screen->GetScale(), screen->GetScale());
    return sprite;
}

int SpriteSheet::GetSpriteSize() const {
    return screen->GetScale() * frame_tile_size;
}

// const int SpriteSheet::GetScale() const { return scale; }

int SpriteSheet::GetFrameTileSize() const { return frame_tile_size; }

const sf::Texture &SpriteSheet::GetTexture() const { return texture; }

const std::vector<sf::Sprite> &SpriteSheet::GetSprites() const {
    return sprites;
}

void SpriteSheet::LoadFromFile() {
    // Some textures may not have a file. This is a noop
    if (texture_path == "") {
        return;
    }

    if (!texture.loadFromFile(texture_path)) {
        std::cout << "Can't laod file" << std::endl;
        exit(1);
    }
}

void SpriteSheet::Reload() { LoadFromFile(); }
