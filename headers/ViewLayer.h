#pragma once

#include <memory>

#include "HouseSceneState.h"
#include "View.h"

class ViewLayer {
    std::vector<std::unique_ptr<View<HouseSceneState>>> views;
    sf::RenderTexture render_texture;
    sf::IntRect texture_size;
    sf::View view;

  public:
    ViewLayer(sf::IntRect, sf::View);
    void Draw(sf::RenderTarget &, const HouseSceneState &);
    void AddView(std::unique_ptr<View<HouseSceneState>> &&view);
    void MoveView(int, int);
    sf::Vector2f GetViewCenter() const;
    void SetViewCenter(sf::Vector2f);
    sf::RenderTexture &GetRenderTexture();
    std::vector<std::unique_ptr<View<HouseSceneState>>> &GetViews();
};
