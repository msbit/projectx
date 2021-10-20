#pragma once

#include <SFML/System/Vector2.hpp>

#include "Entity.h"

class EntityFactory {
  public:
    static Entity Player(sf::Vector2f);
};
