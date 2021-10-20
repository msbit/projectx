#pragma once

#include <SFML/System/Vector2.hpp>

#include "EventWithMouse.h"

struct ControllerSchemeState {
    sf::Vector2f direction;
    bool loot_action_pressed;
    bool attack_pressed;
};

class ControllerScheme {

  public:
    virtual ControllerSchemeState GetInput(const EventWithMouse &) const = 0;
};
