#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics/Rect.hpp>

#include "HouseSceneReducer.h"
#include "Screen.h"

// A map holds all data for a given tile layout within the game.
class Map {
    HouseSceneReducer &reducer;
    std::shared_ptr<Screen> screen;
    int map_tile_pixel_count;
    sf::IntRect map_bounds;

  public:
    Map(HouseSceneReducer &, std::shared_ptr<Screen>, int, sf::IntRect);
    Map(HouseSceneReducer &, std::string, std::shared_ptr<Screen>, int);
    void WriteToFile(std::string) const;
    const sf::FloatRect RoomGridToWorld(sf::IntRect) const;
    const sf::IntRect GetBounds() const;
};
