#include "EntityView.h"
#include "Animation.h"

typedef std::underlying_type<EntityType>::type utype;

EntityView::EntityView(
    const std::shared_ptr<SpriteSheet> entity_sprite_sheet,
    const std::weak_ptr<std::unordered_map<EntityState, Animation>> animations)
    : animations(animations), entity_sprite_sheet(entity_sprite_sheet) {}

void EntityView::Draw(sf::RenderTarget &target,
                      const HouseSceneState &state) const {
    auto entities = state.entities;
    // TODO : There has to be a better way of doing this. It might get very slow
    // to have to search all entites all the time.
    auto found_player =
        std::find_if(entities.begin(), entities.end(), [](const auto &entity) {
            return entity.type == EntityType::PlayerEntity;
        });

    if (found_player == entities.end()) {
        return;
    }

    if (auto shared_animation = animations.lock()) {

        auto current_animation = shared_animation->find(found_player->state);
        if (current_animation == shared_animation->end()) {
            // TODO : We might not want to do this each draw call.
            sf::RectangleShape no_sprite(sf::Vector2f(50, 80));
            no_sprite.setPosition(found_player->transform);
            target.draw(no_sprite);
            return;
        }

        target.draw(current_animation->second.sprite);
    }

    if (state.editor_state.editor_enabled) {
        DrawHitBoxes(*found_player, target);
        DrawPoint(found_player->transform, target);
    }

    for (auto entity : entities) {
        if (entity.type == EntityType::PlayerEntity) {
            return;
        }
        sf::Sprite sprite_to_draw(
            entity_sprite_sheet->GetSprites()[static_cast<utype>(entity.type)]);
        sprite_to_draw.setRotation(entity.rotation);
        sprite_to_draw.setPosition(
            (entity.transform.x * entity_sprite_sheet->GetSpriteSize()),
            (entity.transform.y * entity_sprite_sheet->GetSpriteSize()));
        target.draw(sprite_to_draw);
        if (state.editor_state.editor_enabled) {
            DrawHitBoxes(entity, target);
            DrawPoint(entity.transform, target);
        }
    }
}

void EntityView::DrawHitBoxes(Entity &entity, sf::RenderTarget &target) const {
    for (auto hitbox : entity.hitboxes) {
        auto shape = sf::RectangleShape();
        shape.setSize(sf::Vector2f(hitbox.width, hitbox.height));
        shape.setPosition(entity.transform -
                          sf::Vector2f(hitbox.width / 2, hitbox.height / 2));
        shape.setFillColor(sf::Color(0, 255, 0, 128));
        target.draw(shape);
    }
}

void EntityView::DrawPoint(sf::Vector2f &position,
                           sf::RenderTarget &target) const {
    auto circle_shape = sf::CircleShape(5);
    circle_shape.setFillColor(sf::Color::Red);
    circle_shape.setPosition(position - sf::Vector2f(2.5f, 2.5f));
    target.draw(circle_shape);
}

void EntityView::Update() {}
