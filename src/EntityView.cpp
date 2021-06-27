#include "EntityView.h"
#include "Animation.h"

EntityView::EntityView(
    const SpriteSheet &entity_sprite_sheet,
    const std::weak_ptr<std::unordered_map<EntityState, Animation>> animations)
    : animations(animations), entity_sprite_sheet(entity_sprite_sheet) {}

void EntityView::Draw(sf::RenderTarget &target,
                      const HouseSceneState &state) const {
    auto entities = state.entities;
    // TODO : There has to be a better way of doing this. It might get very slow
    // to have to search all entites all the time.
    auto found_player =
        std::find_if(entities.begin(), entities.end(), [](const auto &entity) {
            return entity.GetEntityType() == EntityType::PlayerEntity;
        });

    if (found_player == entities.end()) {
        return;
    }

    if (auto shared_animation = animations.lock()) {

        auto current_animation =
            shared_animation->find(found_player->GetEntityState());
        if (current_animation == shared_animation->end()) {
            // TODO : We might not want to do this each draw call.
            sf::RectangleShape no_sprite(sf::Vector2f(50, 80));
            no_sprite.setPosition(found_player->GetTransform());
            target.draw(no_sprite);
            return;
        }

        target.draw(current_animation->second.sprite);
    }

    std::for_each(
        entities.begin(), entities.end(), [&target, this](const auto &entity) {
            if (entity.GetEntityType() == EntityType::PlayerEntity) {
                return;
            }
            sf::Sprite sprite_to_draw(
                entity_sprite_sheet.GetSprites()[entity.GetTileMapIndex()]);
            sprite_to_draw.setRotation(entity.GetRotation());
            int half_tile_size = entity_sprite_sheet.GetSpriteSize() / 2;
            sprite_to_draw.setPosition((entity.GetTransform().x *
                                        entity_sprite_sheet.GetSpriteSize()) +
                                           half_tile_size,
                                       (entity.GetTransform().y *
                                        entity_sprite_sheet.GetSpriteSize()) +
                                           half_tile_size);
            sprite_to_draw.setOrigin(entity_sprite_sheet.GetSize() / 2,
                                     entity_sprite_sheet.GetSize() / 2);
            target.draw(sprite_to_draw);
        });
}
