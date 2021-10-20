#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

struct EventWithMouse {
    const sf::Event &event;
    const sf::Vector2i &window_mouse_position;
};
