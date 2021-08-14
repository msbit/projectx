
#pragma once

#include "Controller.h"
#include "EventWithMouse.h"
#include "HouseSceneReducer.h"
#include <unordered_map>

class PlayerController : public Controller<HouseSceneReducer> {
    sf::Vector2f current_input;
    std::weak_ptr<std::unordered_map<EntityState, Animation>> animations;
    sf::Vector2f ClampToRoom(sf::FloatRect, sf::IntRect, sf::Vector2f, sf::Vector2f);

  public:
    PlayerController(std::weak_ptr<std::unordered_map<EntityState, Animation>>);
    void Update(HouseSceneReducer &, sf::Time);
    void HandleInput(const EventWithMouse &, HouseSceneReducer &);
    void Reset();
};
