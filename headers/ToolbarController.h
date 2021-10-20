#pragma once

#include <memory>

#include <SFML/System/Time.hpp>

#include "Controller.h"
#include "EventWithMouse.h"
#include "HouseSceneReducer.h"
#include "Screen.h"

class ToolbarController : public Controller<HouseSceneReducer> {
    std::shared_ptr<Screen> screen;
    void CreateRoom(HouseSceneReducer &);

  public:
    ToolbarController(std::shared_ptr<Screen>);
    void Update(HouseSceneReducer &, sf::Time);
    void HandleInput(const EventWithMouse &, HouseSceneReducer &);
};
