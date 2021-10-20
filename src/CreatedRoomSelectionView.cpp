#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "CreatedRoomSelectionView.h"

void CreatedRoomSelectionView::Draw(sf::RenderTarget &render_target,
                                    const HouseSceneState &state) const {

    if (!state.editor_state.editor_enabled) {
        return;
    }
    // TODO : We should read this from the spirte sheet directly and not hard
    // code it
    auto sprite_size = 16 * 4;
    for (auto &room : state.rooms) {
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setOutlineColor(sf::Color::Yellow);
        rect.setOutlineThickness(1);
        rect.setSize(
            sf::Vector2f(room.width * sprite_size, room.height * sprite_size));
        rect.setPosition(
            sf::Vector2f(room.left * sprite_size, room.top * sprite_size));
        render_target.draw(rect);
    }
}

void CreatedRoomSelectionView::Update() {}
