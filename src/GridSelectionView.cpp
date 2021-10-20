#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "GridSelectionView.h"

GridSelectionView::GridSelectionView(int sprite_size)
    : sprite_size(sprite_size) {}

void GridSelectionView::Draw(sf::RenderTarget &render_target,
                             const HouseSceneState &state) const {
    if (!state.editor_state.editor_enabled) {
        return;
    }

    if (state.editor_state.mouse_dragging) {
        return;
    }

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(1);
    rect.setSize(
        sf::Vector2f(state.editor_state.map_selection.width * sprite_size,
                     state.editor_state.map_selection.height * sprite_size));
    rect.setPosition(state.editor_state.map_selection.left * sprite_size,
                     state.editor_state.map_selection.top * sprite_size);
    render_target.draw(rect);
}

void GridSelectionView::Update() {}
