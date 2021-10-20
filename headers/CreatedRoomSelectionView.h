#pragma once

#include "HouseScene.h"
#include "View.h"

class CreatedRoomSelectionView : public View<HouseSceneState> {
  public:
    void Draw(sf::RenderTarget &, const HouseSceneState &) const;
    void Update();
};
