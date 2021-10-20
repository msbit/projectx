#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "HouseSceneState.h"
#include "View.h"

class GridSelectionView : public View<HouseSceneState> {
    int sprite_size;

  public:
    GridSelectionView(int sprite_size);
    void Draw(sf::RenderTarget &, const HouseSceneState &) const;
    void Update();
};
