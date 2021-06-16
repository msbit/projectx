#pragma once

#include "View.h"
#include "HouseScene.h"

class GridView : public View<HouseSceneState> {
    int size;
public:
    GridView(int);
    void Draw(sf::RenderTarget&, const HouseSceneState&) const;
};
