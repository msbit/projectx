#include <SFML/Graphics/Color.hpp>

#include "SelectedTileView.h"

SelectedTileView::SelectedTileView(std::shared_ptr<SpriteSheet> sprite_sheet)
    : sprite_sheet(sprite_sheet) {}

void SelectedTileView::Draw(sf::RenderTarget &render_target,
                            const HouseSceneState &state) const {
    if (!state.editor_state.editor_enabled) {
        return;
    }

    // Draw Selected Tile. We might need to use a copy constructor here?
    auto current_mouse_grid_position =
        state.editor_state.selected_editor_square;
    auto selected_tile_sprite =
        state.editor_state
            .tile_palette_tiles[state.editor_state.selected_tile_index]
            .icon;
    auto half_tile_size = sprite_sheet->GetSpriteSize() / 2;
    selected_tile_sprite.setPosition(
        (current_mouse_grid_position.x * sprite_sheet->GetSpriteSize()) +
            half_tile_size,
        (current_mouse_grid_position.y * sprite_sheet->GetSpriteSize()) +
            half_tile_size);
    selected_tile_sprite.setColor(sf::Color(255, 255, 255, 170));
    selected_tile_sprite.setOrigin(sprite_sheet->GetFrameTileSize() / 2,
                                   sprite_sheet->GetFrameTileSize() / 2);
    selected_tile_sprite.rotate(state.editor_state.selected_tile_rotation);

    render_target.draw(selected_tile_sprite);
}

void SelectedTileView::Update() {}
