#include <utility>

#include <SFML/Graphics/Sprite.hpp>

#include "ViewLayer.h"

ViewLayer::ViewLayer(sf::IntRect texture_size, sf::View view)
    : texture_size(texture_size), view(view) {
    render_texture.create(texture_size.width, texture_size.height);
}

void ViewLayer::Draw(sf::RenderTarget &render_target,
                     const HouseSceneState &house_scene_state) {
    render_texture.setView(view);
    render_texture.clear();
    for (auto &view : views) {
        view->Draw(render_texture, house_scene_state);
    }

    render_texture.display();
    sf::Sprite texture_sprite(render_texture.getTexture());
    texture_sprite.setPosition(texture_size.left, texture_size.top);
    render_target.draw(texture_sprite);
}

sf::RenderTexture &ViewLayer::GetRenderTexture() { return render_texture; }

void ViewLayer::AddView(std::unique_ptr<View<HouseSceneState>> &&view) {
    views.push_back(std::move(view));
}

sf::Vector2f ViewLayer::GetViewCenter() const { return view.getCenter(); };

void ViewLayer::MoveView(int x, int y) { view.move(x, y); }

std::vector<std::unique_ptr<View<HouseSceneState>>> &ViewLayer::GetViews() {
    return views;
}

void ViewLayer::SetViewCenter(sf::Vector2f new_center) {
    view.setCenter(new_center);
}
