#pragma once

#include <unordered_map>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "Animation.h"

/*
 * Template specialisation of `std::hash<Key>(Key &)` to allow
 * use of sf::Keyboard::Key as the key type in `unordered_set`:
 *
 *  https://en.cppreference.com/w/cpp/language/template_specialization
 *
 * Use of `static_cast` is valid for converting scoped enums to
 * integer types:
 *
 *  https://en.cppreference.com/w/cpp/language/static_cast
 */
namespace std {
    template <> struct hash<sf::Keyboard::Key> {
        size_t operator()(const sf::Keyboard::Key &k) const {
            return static_cast<std::size_t>(k);
        }
    };
}

class Entity {
    sf::Vector2f transform;
    sf::Vector2f direction;

    // TODO : This should move out somewhere that can handle keyboards, control pads and other custom bindings
    sf::Vector2f controller;
    std::unordered_set<sf::Keyboard::Key> heldKeys;
    float speed;
    float acceleration;
    bool facing_left;
    std::unordered_map<std::string, Animation> animations;

public:
    Entity(float speed, float acceleration);
    Entity(float speed, float acceleration, std::unordered_map<std::string, Animation>&);
    sf::Vector2f getTransform();
    void Update(const sf::Event &);
    void Draw(sf::RenderTarget &);
    void Reset();
};


