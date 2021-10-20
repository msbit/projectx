#pragma once

#include <HouseScene.h>

class LightingView : public View<HouseSceneState> {
    sf::Shader frag_shader;
    sf::RenderTexture texture;

  public:
    void Draw(sf::RenderTarget &render_target,
              const HouseSceneState &state) const;
};
