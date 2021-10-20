#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>

#include "Animation.h"
#include "AssetWatcher.h"
#include "Controller.h"
#include "Entity.h"
#include "EventWithMouse.h"
#include "HouseSceneReducer.h"
#include "ViewLayer.h"

typedef std::map<std::string, std::shared_ptr<ViewLayer>> ViewLayerMap;

typedef std::shared_ptr<std::unordered_map<EntityState, Animation>>
    AnimationMap;

struct TimedController {
    sf::Clock timer;
    std::shared_ptr<Controller<HouseSceneReducer>> controller;
    TimedController(std::shared_ptr<Controller<HouseSceneReducer>> controller)
        : controller(controller) {}
};

class HouseScene {
    std::shared_ptr<AssetWatcher> asset_watcher;
    std::vector<std::shared_ptr<TimedController>> timed_controllers;
    ViewLayerMap view_layers;
    AnimationMap animations;
    std::shared_ptr<HouseSceneReducer> reducer;

  public:
    HouseScene(std::shared_ptr<AssetWatcher>,
               std::vector<std::shared_ptr<TimedController>>, ViewLayerMap,
               AnimationMap, std::shared_ptr<HouseSceneReducer>);
    void HandleInput(const EventWithMouse &);
    void Update();
    void Draw(sf::RenderTarget &window);
};
