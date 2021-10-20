#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "HouseSceneState.h"
#include "Screen.h"
#include "View.h"

class TilePaletteView : public View<HouseSceneState> {
    sf::RenderTexture tile_palette_render_texture;
    std::shared_ptr<Screen> screen;

  public:
    TilePaletteView(int, int, std::shared_ptr<Screen>);
    void Draw(sf::RenderTarget &, const HouseSceneState &) const;
    void Update();
};
