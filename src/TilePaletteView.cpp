#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "TilePaletteView.h"

TilePaletteView::TilePaletteView(int width, int height,
                                 std::shared_ptr<Screen> screen)
    : screen(screen) {
    tile_palette_render_texture.create(width, height);
}

void TilePaletteView::Draw(sf::RenderTarget &target,
                           const HouseSceneState &state) const {
    if (!state.editor_state.editor_enabled) {
        return;
    }

    auto bg = screen->GetTilePaletteArea();
    sf::RectangleShape tile_palette_background;
    tile_palette_background.setFillColor(sf::Color(60, 60, 60, 255));
    tile_palette_background.setPosition(sf::Vector2f(bg.left, bg.top));
    tile_palette_background.setSize(sf::Vector2f(bg.width, bg.height));

    target.draw(tile_palette_background);
    for (const auto &tile : state.editor_state.tile_palette_tiles) {
        target.draw(tile.icon);
    }
    target.draw(state.editor_state.tile_palette_selection_rectangle);
}

void TilePaletteView::Update() {}
