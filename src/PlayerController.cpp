#include <algorithm>

#include "PlayerController.h"

PlayerController::PlayerController(
    std::shared_ptr<std::unordered_map<EntityState, Animation>> animations,
    std::shared_ptr<ControllerScheme> controller_scheme,
    std::shared_ptr<Map> map, ViewLayer &house_view)
    : animations(animations), controller_scheme(controller_scheme), map(map),
      house_view(house_view) {}

void PlayerController::HandleInput(const EventWithMouse &event_with_mouse,
                                   HouseSceneReducer &) {
    current_input = controller_scheme->GetInput(event_with_mouse);
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

    auto acceleration = sf::Vector2f(0, 0);
    auto const force =
        (delta_time.asMilliseconds() * found_player->acceleration) / 1000;

    acceleration += sf::Vector2f(force * current_input.direction.x,
                                 force * current_input.direction.y);

    auto new_velocity = found_player->velocity + acceleration;
    // TODO : Note this is a damping const of 0.1. We most likley
    auto const new_damping =
        sf::Vector2f(-new_velocity.x, -new_velocity.y) * 0.1f;

    new_velocity += new_damping;
    reducer.SetEntityVelocity(new_velocity);

    auto rooms = reducer.GetState().rooms;

    // TODO : Remove Magic number
    // auto sprite_size = reducer.GetState().scale * 16;

    auto room_the_player_is_in = std::find_if(
        rooms.begin(), rooms.end(), [this, found_player](const auto &room) {
            return map->RoomGridToWorld(room).contains(found_player->transform);

            /*
            return sf::FloatRect(
                room.left * sprite_size,
                room.top * sprite_size,
                room.width * sprite_size,
                room.height * sprite_size
            ).contains(found_player->transform);
            */
        });

    if (room_the_player_is_in == rooms.end() ||
        found_player->hitboxes.size() < 1) {
        reducer.SetEntityTransform(found_player->transform + new_velocity);
    } else {
        /*

        auto scaled_room =
            sf::IntRect(room_the_player_is_in->left * sprite_size,
                          room_the_player_is_in->top * sprite_size,
                          room_the_player_is_in->width * sprite_size,
                          room_the_player_is_in->height * sprite_size);
        */

        auto scaled_room = map->RoomGridToWorld(*room_the_player_is_in);

        reducer.SetEntityTransform(ClampToRoom(
            found_player->hitboxes[0], // TODO : Not sure what to do when we
                                       // have more than one hitbox
            scaled_room, found_player->transform,
            found_player->transform + new_velocity));
    }

    if (new_velocity.x > 0.01f || new_velocity.x < -0.01f ||
        new_velocity.y > 0.01f || new_velocity.y < -0.01f) {
        reducer.SetPlayerState(EntityState::Walking);
    } else {
        reducer.SetPlayerState(EntityState::Idle);
    }

    if (!reducer.GetState().editor_state.editor_enabled) {
        house_view.SetViewCenter(found_player->transform);
    }

    /*

    // Not sure if this should go in the reducer but we can put it here for now
    auto input_mag =
        std::sqrt(std::pow(current_input.direction.x, 2) +
    std::pow(current_input.direction.y, 2)); auto norm_vector =
        sf::Vector2f(std::round(current_input.direction.x / input_mag),
    std::round(current_input.direction.y / input_mag));

    if (!std::isnan(norm_vector.x) || !std::isnan(norm_vector.y)) {
        reducer.SetPlayerDirection(norm_vector);
    }

    */

    reducer.SetPlayerDirection(current_input.direction);
    HandleActions(reducer);

    /*
    TODO : This most likely won't work as different entities will move the
    animations around before they are drawn. We need to also figure out where
    this should end up living but TBH it feels like it should be in the view
    */
    auto state = reducer.GetState();
    auto current_animation = animations->find(found_player->state);
    current_animation->second.sprite.setPosition(found_player->transform);
    current_animation->second.sprite.setRotation(found_player->facing);
}

void PlayerController::HandleActions(HouseSceneReducer &) {}

void PlayerController::Reset() {
    // entity.SetVelocity(sf::Vector2f(0, 0));
}

sf::Vector2f PlayerController::ClampToRoom(sf::FloatRect player_hitbox,
                                           sf::FloatRect room,
                                           sf::Vector2f original_position,
                                           sf::Vector2f new_position) {

    auto offset_hitbox =
        sf::FloatRect(new_position.x - player_hitbox.width / 2,
                      new_position.y - player_hitbox.height / 2,
                      player_hitbox.width, player_hitbox.height);

    // clamp left
    if (offset_hitbox.left < room.left) {
        new_position.x = original_position.x;
    }

    // clamp right
    if (offset_hitbox.left + player_hitbox.width > room.left + room.width) {
        new_position.x = original_position.x;
    }

    // clamp top
    if (offset_hitbox.top < room.top) {
        new_position.y = original_position.y;
    }

    // clamp bottom
    if (offset_hitbox.top + offset_hitbox.height > room.top + room.height) {
        new_position.y = original_position.y;
    }

    return new_position;
}
