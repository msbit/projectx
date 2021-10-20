#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "Entity.h"

enum class TilePaletteType { PaletteTile, PaletteEntity };

struct TilePaletteTile {
    sf::Sprite icon;
    TilePaletteType type;
    EntityType entity_type;
};
