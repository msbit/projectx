#include <cstdlib>
#include <iostream>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "ShadowView.h"

ShadowView::ShadowView(sf::IntRect map_size) {
    if (!sf::Shader::isAvailable()) {
        std::cout << "Shaders not avaiable" << std::endl;
        std::exit(1);
    }

    if (!frag_shader.loadFromFile("shaders/shadow.fs", sf::Shader::Fragment)) {
        std::cout << "Error loading Shader" << std::endl;
        std::exit(1);
    }

    render_texture.create(map_size.width, map_size.height);

    frag_shader.setUniform("texture", sf::Shader::CurrentTexture);
}

void ShadowView::Update() { render_texture.display(); }

void ShadowView::Draw(sf::RenderTarget &render_target,
                      const HouseSceneState &state) const {
    if (state.editor_state.editor_enabled) {
        return;
    }
    sf::Sprite texture_sprite(render_texture.getTexture());
    texture_sprite.setPosition(sf::Vector2f(0, 0));
    render_target.draw(texture_sprite, &frag_shader);
}
