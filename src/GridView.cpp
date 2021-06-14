#include "GridView.h"

GridView::GridView(int size) : size(size) {}

void GridView::Draw(sf::RenderTarget &target, const HouseSceneState& state) const {
    if (!state.editor_state.editor_enabled) {
        return;
    }

    auto grid_height = state.map_bounds.height;
    auto grid_width = state.map_bounds.width;

    auto total_grid_height = grid_height * size;
    auto total_grid_width = grid_width * size;

    auto line_color = sf::Color(255,255,255, 50);

    for(auto row = 1; row < (grid_height +1) * size; row = row + size) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, row), line_color),
            sf::Vertex(sf::Vector2f(total_grid_width, row), line_color)
        };
        target.draw(line, 2, sf::Lines);
    }

    for(auto col = 1; col < (grid_width + 1) * size; col = col + size) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(col, 0), line_color),
            sf::Vertex(sf::Vector2f(col, total_grid_height), line_color)
        };
        target.draw(line, 2, sf::Lines);
    }
}
