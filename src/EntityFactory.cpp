#include <SFML/Graphics/Rect.hpp>

#include "EntityFactory.h"

Entity EntityFactory::Player(sf::Vector2f position) {

    // Here now we know we have a valid state we execute an action to load the
    // map. Note here that if we ever intend to dispatch these actions more than
    // once they should be added to a controller
    // auto player_hitbox = sf::FloatRect(-55, -25, 50, 120);
    auto player_hitbox = sf::FloatRect(0, 0, 50, 120);

    return Entity{EntityType::PlayerEntity,
                  500.f,
                  30.f,
                  position,
                  sf::Vector2f(0, 0),
                  sf::Vector2f(0, 0),
                  EntityState::Idle,
                  0,
                  {player_hitbox}};
}
