#pragma once

#include <SFML/Graphics.hpp>

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
