#pragma once

#include "Entity.h"
#include "HouseScene.h"

class EntityView : public View<HouseSceneState> {
    const std::weak_ptr<std::unordered_map<EntityState, Animation>> animations;
    const std::shared_ptr<SpriteSheet> entity_sprite_sheet;
    void DrawHitBoxes(Entity &, sf::RenderTarget &) const;
    void DrawPoint(sf::Vector2f &, sf::RenderTarget &) const;

  public:
    EntityView(const std::shared_ptr<SpriteSheet>,
               const std::weak_ptr<std::unordered_map<EntityState, Animation>>);
    void Draw(sf::RenderTarget &render_target,
              const HouseSceneState &state) const;
    void Update();
};
