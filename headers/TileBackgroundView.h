#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include "HouseSceneState.h"
#include "SpriteSheet.h"
#include "View.h"

class TileBackgroundView : public View<HouseSceneState> {
    const std::shared_ptr<SpriteSheet> tile_map;

  public:
    TileBackgroundView(std::shared_ptr<SpriteSheet>);
    void Draw(sf::RenderTarget &, const HouseSceneState &) const;
    void Update();
};
