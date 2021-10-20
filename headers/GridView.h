#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "HouseSceneState.h"
#include "Map.h"
#include "View.h"

class GridView : public View<HouseSceneState> {
    int size;
    Map &map;

  public:
    GridView(int, Map &);
    void Draw(sf::RenderTarget &, const HouseSceneState &) const;
    void Update();
};
