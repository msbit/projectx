#include "PlayerController.h"

PlayerController::PlayerController(
    std::weak_ptr<std::unordered_map<EntityState, Animation>> animations)
    : animations(animations) {}

void PlayerController::HandleInput(const EventWithMouse &event_with_mouse,
                                   HouseSceneReducer &state) {

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Left) {
        current_input = sf::Vector2f(-1, 0);
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Right) {
        current_input = sf::Vector2f(1, 0);
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Up) {
        current_input = sf::Vector2f(0, -1);
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Down) {
        current_input = sf::Vector2f(0, 1);
    }

    if (event_with_mouse.event.type == sf::Event::KeyReleased) {
        current_input = sf::Vector2f(0, 0);
    }
}

void PlayerController::Update(HouseSceneReducer &reducer, sf::Time delta_time) {
    auto entities = reducer.GetState().entities;

    // TODO : There has to be a better way of doing this. It might get very slow
    // to have to search all entites all the time.
    auto found_player =
        std::find_if(entities.begin(), entities.end(), [](const auto &entity) {
            return entity.type == EntityType::PlayerEntity;
        });

    if (found_player == entities.end()) {
        return;
    }

    auto acceleration = found_player->acceleration;
    auto player_velocity = found_player->velocity;
    auto speed = found_player->speed;
    auto new_velocity = sf::Vector2f(
        ((current_input.x * speed) - player_velocity.x) * acceleration,
        ((current_input.y * speed) - player_velocity.y) * acceleration);

    reducer.SetEntityVelocity(new_velocity);
    reducer.SetEntityTransform(found_player->transform + new_velocity);

    if (!reducer.GetState().editor_state.editor_enabled) {
        reducer.SetHouseViewCenter(found_player->transform);
    }

    if (new_velocity.x != 0 || new_velocity.y != 0) {
        reducer.SetPlayerState(EntityState::Walking);
    } else {
        reducer.SetPlayerState(EntityState::Idle);
    }

    /*
    if (facing_left && new_velocity.x > 0) {
        facing_left = false;
    }

    if (!facing_left && new_velocity.x < 0) {
        facing_left = true;
    }
    */

    // TODO : This most likely won't work as different entities will move the
    // animations around before they are drawn
    if (auto shared_animations = animations.lock()) {

        auto state = reducer.GetState();
        auto found_player =
            std::find_if(state.entities.begin(), state.entities.end(),
                         [](const auto &entity) {
                             return entity.type == EntityType::PlayerEntity;
                         });

        if (found_player != state.entities.end()) {
            auto current_animation =
                shared_animations->find(found_player->state);
            current_animation->second.sprite.setPosition(
                found_player->transform);
        }

        /*
        if (facing_left) {
            current_animation->second.sprite.setScale(
                -4, current_animation->second.sprite.getScale().y);
        } else {
            current_animation->second.sprite.setScale(
                4, current_animation->second.sprite.getScale().y);
        }
        */
    }
}

void PlayerController::Reset() {
    std::cout << "PlayerController::Reset not implemented yet" << std::endl;
    // entity.SetVelocity(sf::Vector2f(0, 0));
}
