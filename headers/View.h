#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

template <typename T> class View {
  public:
    virtual ~View() = default;
    virtual void Update() = 0;
    virtual void Draw(sf::RenderTarget &, const T &) const = 0;
};
