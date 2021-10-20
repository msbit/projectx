#pragma once

#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.h"
#include "Controller.h"
#include "ControllerScheme.h"
#include "Entity.h"
#include "EventWithMouse.h"
#include "HouseSceneReducer.h"
#include "Map.h"
#include "ViewLayer.h"

class PlayerController : public Controller<HouseSceneReducer> {
    ControllerSchemeState current_input;
    std::shared_ptr<std::unordered_map<EntityState, Animation>> animations;
    sf::Vector2f ClampToRoom(sf::FloatRect, sf::FloatRect, sf::Vector2f,
                             sf::Vector2f);
    std::shared_ptr<ControllerScheme> controller_scheme;
    void HandleActions(HouseSceneReducer &);
    std::shared_ptr<Map> map;
    ViewLayer &house_view;

  public:
    PlayerController(
        std::shared_ptr<std::unordered_map<EntityState, Animation>>,
        std::shared_ptr<ControllerScheme>, std::shared_ptr<Map> map,
        ViewLayer &);
    void Update(HouseSceneReducer &, sf::Time);
    void HandleInput(const EventWithMouse &, HouseSceneReducer &);
    void Reset();
};
