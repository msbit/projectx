#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

enum class EntityType { GhostEntity = 0, DoorEntity = 1, PlayerEntity = 2 };

enum class EntityState { Idle, Throwing, Walking, Attacking, Dying };

struct Entity {
    EntityType type;
    float speed;
    float acceleration;
    sf::Vector2f transform;
    sf::Vector2f direction = sf::Vector2f(1, 1);
    sf::Vector2f velocity;
    EntityState state;
    int rotation;
    std::vector<sf::FloatRect> hitboxes;
    double facing;
};
