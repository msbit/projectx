#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "HouseSceneState.h"
#include "View.h"

class LightingView : public View<HouseSceneState> {
    sf::Shader frag_shader;
    sf::RenderTexture texture;

  public:
    void Draw(sf::RenderTarget &render_target,
              const HouseSceneState &state) const;
};
