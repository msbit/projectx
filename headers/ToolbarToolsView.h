#pragma once

#include "HouseSceneState.h"
#include "View.h"

class ToolbarToolsView : public View<HouseSceneState> {
    int window_width;
    const std::shared_ptr<SpriteSheet> tool_map;
    std::shared_ptr<Screen> screen;

  public:
    ToolbarToolsView(std::shared_ptr<SpriteSheet>, std::shared_ptr<Screen>);
    void Draw(sf::RenderTarget &, const HouseSceneState &) const;
    void Update();
};
