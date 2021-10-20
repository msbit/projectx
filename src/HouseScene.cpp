#include <memory>
#include <unordered_map>

#include "HouseScene.h"

HouseScene::HouseScene(
    std::shared_ptr<AssetWatcher> asset_watcher,
    std::vector<std::shared_ptr<TimedController>> timed_controllers,
    ViewLayerMap view_layers, AnimationMap animations,
    std::shared_ptr<HouseSceneReducer> reducer)
    : asset_watcher(asset_watcher), timed_controllers(timed_controllers),
      view_layers(view_layers), animations(animations), reducer(reducer) {}

void HouseScene::HandleInput(const EventWithMouse &event) {
    for (auto &timed_controller : timed_controllers)
        timed_controller->controller->HandleInput(event, *reducer);
}

void HouseScene::Update() {

    asset_watcher->ReloadIfRequired();

    for (auto &timed_controller : timed_controllers)
        timed_controller->controller->Update(*reducer,
                                             timed_controller->timer.restart());

    for (auto &animation : *animations)
        animation.second.Update();

    for (auto &view : view_layers["house_map"]->GetViews())
        view->Update();

    for (auto &view : view_layers["toolbar"]->GetViews())
        view->Update();

    for (auto &view : view_layers["tile_palette"]->GetViews())
        view->Update();
}

void HouseScene::Draw(sf::RenderTarget &render_target) {
    auto state = reducer->GetState();
    view_layers["house_map"]->Draw(render_target, state);
    view_layers["toolbar"]->Draw(render_target, state);
    view_layers["tile_palette"]->Draw(render_target, state);
}
