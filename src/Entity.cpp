#include "Entity.h"

Entity::Entity(float speed, float acceleration) : 
    speed(speed), 
    acceleration(acceleration), 
    animations(),
    facing_left(true) {}

Entity::Entity(
    float speed, 
    float acceleration,
    std::unordered_map<std::string, Animation>& animations
) : speed(speed), acceleration(acceleration), animations(animations) {}

void Entity::Update(const sf::Event &event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
        heldKeys.insert(event.key.code);

        switch (event.key.code) {
        case sf::Keyboard::Left:
            controller.x = -1;
            break;

        case sf::Keyboard::Right:
            controller.x = 1;
            break;

        case sf::Keyboard::Up:
            controller.y = -1;
            break;

        case sf::Keyboard::Down:
            controller.y = 1;
            break;

        default:
            break;
        }

        break;

    case sf::Event::KeyReleased:
        heldKeys.erase(event.key.code);

        switch (event.key.code) {
        case sf::Keyboard::Left:
            controller.x = heldKeys.count(sf::Keyboard::Right) ? 1 : 0;
            break;

        case sf::Keyboard::Right:
            controller.x = heldKeys.count(sf::Keyboard::Left) ? -1 : 0;
            break;

        case sf::Keyboard::Up:
            controller.y = heldKeys.count(sf::Keyboard::Down) ? 1 : 0;
            break;

        case sf::Keyboard::Down:
            controller.y = heldKeys.count(sf::Keyboard::Up) ? -1 : 0;
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }

}

void Entity::Draw(sf::RenderTarget &target) {
    direction.x = ((controller.x * speed) - direction.x) * acceleration;
    direction.y = ((controller.y * speed) - direction.y) * acceleration;
    transform = transform + direction;

    std::string current_animation_key = "idle";

    if (controller.x != 0 || controller.y != 0) {
        current_animation_key = "walk";
    } 

    auto current_animation = animations.find(current_animation_key);

    if (facing_left && controller.x > 0) {
        facing_left = false;
    }

    if (!facing_left && controller.x < 0) {
        facing_left = true;
    }

    if (facing_left) {
        current_animation->second.sprite.setScale(
            -4,
            current_animation->second.sprite.getScale().y
        );
    } else {
        current_animation->second.sprite.setScale(
            4,
            current_animation->second.sprite.getScale().y
        );
    } 

    if(current_animation != animations.end()) {
        current_animation->second.Update();
        current_animation->second.sprite.setPosition(transform);
        target.draw(current_animation->second.sprite);
    } else {
        // TODO : We might not want to do this each draw call.
        sf::RectangleShape no_sprite(sf::Vector2f(50,80));
        no_sprite.setPosition(transform);
        target.draw(no_sprite);
    }

}


sf::Vector2f Entity::getTransform() {
    return transform;
}

void Entity::Reset() {
    heldKeys.clear();

    controller.x = 0;
    controller.y = 0;
}
