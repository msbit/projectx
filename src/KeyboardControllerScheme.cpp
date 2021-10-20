#include "KeyboardControllerScheme.h"

ControllerSchemeState KeyboardControllerScheme::GetInput(
    const EventWithMouse &event_with_mouse) const {
    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Left) {
        return {sf::Vector2f(-1, 0), false, false};
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Right) {
        return {sf::Vector2f(1, 0), false, false};
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Up) {
        return {sf::Vector2f(0, -1), false, false};
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Down) {
        return {sf::Vector2f(0, 1), false, false};
    }

    if (event_with_mouse.event.type == sf::Event::KeyReleased) {
        return {sf::Vector2f(0, 0), false, false};
    }
}