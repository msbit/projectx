#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Joystick.hpp>

#include "GamepadControllerScheme.h"

const int PLAYER_ONE_JOYSTICK_ID = 1;
const int LOOT_ACTION_BUTTON_ID = 1;
const int ATTACK_ACTION_BUTTON_ID = 2;

ControllerSchemeState
GamepadControllerScheme::GetInput(const EventWithMouse &) const {

    float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100;
    float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100;
    bool loot_action_pressed = sf::Joystick::isButtonPressed(
        PLAYER_ONE_JOYSTICK_ID, LOOT_ACTION_BUTTON_ID);

    bool attack_pressed = sf::Joystick::isButtonPressed(
        PLAYER_ONE_JOYSTICK_ID, ATTACK_ACTION_BUTTON_ID);

    float threshhold = .08f;

    if ((x < threshhold && x > 0) || (x > threshhold * -1 && x < 0)) {
        x = 0;
    }

    if ((y < threshhold && y > 0) || (y > threshhold * -1 && y < 0)) {
        y = 0;
    }

    return {sf::Vector2f(x, y), loot_action_pressed, attack_pressed};
}
