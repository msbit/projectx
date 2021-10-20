#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "BoxSelectionView.h"

void BoxSelectionView::Draw(sf::RenderTarget &render_target,
                            const HouseSceneState &state) const {
    if (!state.editor_state.mouse_dragging) {
        return;
    }

    auto delta = sf::Vector2f((state.editor_state.mouse_down_at.x -
                               state.editor_state.current_mouse_position.x) *
                                  -1,
                              (state.editor_state.mouse_down_at.y -
                               state.editor_state.current_mouse_position.y) *
                                  -1);

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1);
    rect.setSize(delta);
    rect.setPosition(state.editor_state.mouse_down_at);
    render_target.draw(rect);
}

void BoxSelectionView::Update() {}
