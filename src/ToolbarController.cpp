#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "ToolbarController.h"

ToolbarController::ToolbarController(std::shared_ptr<Screen> screen)
    : screen(screen){};

void ToolbarController::Update(HouseSceneReducer &, sf::Time) {}

void ToolbarController::HandleInput(const EventWithMouse &event_with_mouse,
                                    HouseSceneReducer &reducer) {
    if (event_with_mouse.event.type == sf::Event::MouseButtonReleased &&
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left) {
        const auto state = reducer.GetState();

        auto first_button = screen->GetButtonArea();

        if (first_button.contains(event_with_mouse.event.mouseButton.x,
                                  event_with_mouse.event.mouseButton.y)) {
            CreateRoom(reducer);
        }
    }
}

void ToolbarController::CreateRoom(HouseSceneReducer &reducer) {
    auto const current_state = reducer.GetState();
    reducer.AddRoom(current_state.editor_state.map_selection);
    reducer.ClearEditorSelection();
}
