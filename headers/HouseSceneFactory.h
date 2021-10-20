#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include "AssetWatcher.h"
#include "ControllerScheme.h"
#include "HouseScene.h"
#include "HouseSceneReducer.h"
#include "Map.h"
#include "Screen.h"

class HouseSceneFactory {
    static std::shared_ptr<AssetWatcher>
    InitAssetWatcher(int, int, std::shared_ptr<Screen>);

    static std::shared_ptr<Screen> InitScreen(const int, const int, const int);

    static AnimationMap InitAnimations(std::shared_ptr<AssetWatcher>,
                                       std::shared_ptr<Screen>);

    static ViewLayerMap InitViewLayers(std::shared_ptr<AssetWatcher>,
                                       std::shared_ptr<Screen>, AnimationMap,
                                       sf::IntRect, Map &);

    static std::vector<std::shared_ptr<TimedController>>
        InitControllers(std::shared_ptr<AssetWatcher>, AnimationMap,
                        std::shared_ptr<Screen>, std::shared_ptr<Map>,
                        ViewLayerMap);

    static std::shared_ptr<ControllerScheme> InitControllerScheme();

    static std::shared_ptr<Map> InitMap(const std::optional<std::string>,
                                        HouseSceneReducer &,
                                        std::shared_ptr<Screen>, int);

  public:
    static std::unique_ptr<HouseScene> Init(const int, const int,
                                            const std::optional<std::string>);
};
