#pragma once

#include "HouseScene.h"
#include "Entity.h"

class EntityView : public View<HouseSceneState> {
    const std::unordered_map<EntityState, Animation>& animations;
    const SpriteSheet& entity_sprite_sheet;
public:
    EntityView(
        const SpriteSheet&,
        const std::unordered_map<EntityState, Animation>&
    );
    void Draw(sf::RenderTarget& render_target, const HouseSceneState& state) const;
};

