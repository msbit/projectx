#include <SFML/Graphics.hpp>

#include "ToolbarToolsView.h"

ToolbarToolsView::ToolbarToolsView(std::shared_ptr<SpriteSheet> tool_map,
                                   std::shared_ptr<Screen> screen)
    : tool_map(tool_map), screen(screen) {}

void ToolbarToolsView::Draw(sf::RenderTarget &render_target,
                            const HouseSceneState &state) const {
    if (!state.editor_state.editor_enabled) {
        return;
    }

    auto toolbar_icon_padding = screen->GetToolbarIconPadding();
    sf::RectangleShape background;
    background.setPosition(0, 0);
    background.setSize(
        sf::Vector2f(screen->GetWindowSize().width,
                     (toolbar_icon_padding * 2) + tool_map->GetSpriteSize()));
    background.setFillColor(sf::Color(60, 60, 60, 255));
    render_target.draw(background);

    int current_tool_count = 0;
    for (auto &tool : state.editor_state.tools) {
        sf::Sprite sprite_to_draw(
            tool_map->GetSprites()[tool.sprite_map_index]);
        sprite_to_draw.setPosition(toolbar_icon_padding + (current_tool_count),
                                   toolbar_icon_padding);
        render_target.draw(sprite_to_draw);
        current_tool_count++;
    }
}

void ToolbarToolsView::Update() {}
