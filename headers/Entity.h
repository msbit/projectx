#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Animation.h"

enum class EntityType { 
    GhostEntity = 0, 
    DoorEntity = 1, 
    PlayerEntity = 2
};

enum class EntityMode {
    Idle,
    Throwing,
    Walking,
    Attacking,
    Dying
};

class Entity {
    float acceleration;
    std::unordered_map<EntityMode, Animation> animations;
    // TODO : This should move out somewhere that can handle keyboards, control pads and other custom bindings
    sf::Vector2f controller;
    sf::Vector2f direction;
    bool facing_left;
    float speed;
    sf::Vector2f transform;
    EntityType type;

public:
    Entity(EntityType, float, float, float, float);
    Entity(EntityType, float, float, std::unordered_map<EntityMode, Animation>&);
    void Update(const sf::Event &);
    void Draw(sf::RenderTarget &);
    void Reset();
    const sf::Vector2f& GetTransform() const; 
    const int GetTileMapIndex() const; 
    const int GetRotation() const; 
    const EntityType GetEntityType () const; 
};


