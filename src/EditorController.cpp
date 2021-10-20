#include <math.h>

#include "EditorController.h"

EditorController::EditorController(
    int tile_map_sprite_size, sf::RenderTexture &tile_palette_render_texture,
    sf::RenderTexture &house_render_texture, Map &map,
    std::shared_ptr<Screen> screen, ViewLayer &house_view_layer,
    ViewLayer &tile_palette_view)
    : tile_map_sprite_size(tile_map_sprite_size),
      tile_palette_render_texture(tile_palette_render_texture),
      house_render_texture(house_render_texture), map(map), screen(screen),
      house_view_layer(house_view_layer), tile_palette_view(tile_palette_view) {
}

void EditorController::Update(HouseSceneReducer &reducer, sf::Time) {
    auto state = reducer.GetState();

    auto selected_tile_position =
        state.editor_state
            .tile_palette_tiles[state.editor_state.selected_tile_index]
            .icon.getPosition();

    reducer.SetSelectionRectanglePosition(selected_tile_position);
}

void EditorController::HandleInput(const EventWithMouse &event_with_mouse,
                                   HouseSceneReducer &reducer) {

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::E) {
        reducer.ToggleEditorEnabled();
        reducer.ResetPlayer();
        return;
    }

    if (!reducer.GetState().editor_state.editor_enabled) {
        return;
    }

    if (event_with_mouse.event.type == sf::Event::MouseButtonReleased &&
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left) {

        auto event_target_coords = tile_palette_render_texture.mapPixelToCoords(
            sf::Vector2i(event_with_mouse.event.mouseButton.x,
                         event_with_mouse.event.mouseButton.y));
        auto found = std::find_if(
            reducer.GetState().editor_state.tile_palette_tiles.begin(),
            reducer.GetState().editor_state.tile_palette_tiles.end(),
            [event_target_coords](const auto &t) {
                return t.icon.getGlobalBounds().contains(event_target_coords);
            });

        if (found != reducer.GetState().editor_state.tile_palette_tiles.end()) {
            reducer.UpdateSelectedTileIndex(
                found -
                reducer.GetState().editor_state.tile_palette_tiles.begin());
        }
    }

    auto tile_palette_bounds = screen->GetTilePaletteArea();
    if (event_with_mouse.event.type == sf::Event::MouseWheelMoved &&
        event_with_mouse.event.mouseButton.x < tile_palette_bounds.width) {
        int upper_scroll_center = tile_palette_bounds.height / 2;

        int lower_scroll_center =
            screen->GetTilePaletteArea().height - upper_scroll_center;

        // TODO : Combile and clean up these if statements

        if (event_with_mouse.event.mouseWheel.delta < 0 &&
            tile_palette_view.GetViewCenter().y > upper_scroll_center) {

            tile_palette_view.MoveView(
                0, 100 * event_with_mouse.event.mouseWheel.delta);
        }

        if (event_with_mouse.event.mouseWheel.delta > 0 &&
            tile_palette_view.GetViewCenter().y < lower_scroll_center) {

            tile_palette_view.MoveView(
                0, 100 * event_with_mouse.event.mouseWheel.delta);
        }
    }

    if (reducer.GetState().editor_state.editor_enabled) {
        auto current_target_coords = house_render_texture.mapPixelToCoords(
            event_with_mouse.window_mouse_position);

        reducer.UpdateSelectedEditorSquare(sf::Vector2i(
            floor(current_target_coords.x / tile_map_sprite_size),
            floor(current_target_coords.y / tile_map_sprite_size)));
    }

    if (event_with_mouse.event.type == sf::Event::MouseButtonReleased &&
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left &&
        !reducer.GetState().editor_state.mouse_dragging) {

        sf::Vector2f event_target_coords =
            house_render_texture.mapPixelToCoords(
                sf::Vector2i(event_with_mouse.event.mouseButton.x,
                             event_with_mouse.event.mouseButton.y));

        sf::IntRect pixel_bounds(0, 0,
                                 map.GetBounds().width * tile_map_sprite_size,
                                 map.GetBounds().height * tile_map_sprite_size);

        if (pixel_bounds.contains(event_target_coords.x,
                                  event_target_coords.y) &&
            !screen->GetTilePaletteArea().contains(
                event_with_mouse.event.mouseButton.x,
                event_with_mouse.event.mouseButton.y)) {
            auto x = (int)event_target_coords.x / tile_map_sprite_size;
            auto y = (int)event_target_coords.y / tile_map_sprite_size;

            auto selected_tile =
                reducer.GetState().editor_state.tile_palette_tiles
                    [reducer.GetState().editor_state.selected_tile_index];

            if (selected_tile.type == TilePaletteType::PaletteTile) {
                reducer.AddTile(x, y);
            } else if (selected_tile.type == TilePaletteType::PaletteEntity) {
                reducer.AddEntity(x, y);
            } else {
                std::cout << "Map type not supported" << std::endl;
                exit(1);
            }
        }
    }

    if (event_with_mouse.event.type == sf::Event::KeyReleased &&
        event_with_mouse.event.key.code == sf::Keyboard::W) {
        map.WriteToFile("./assets/maps/room.bin");
    }

    if (event_with_mouse.event.type == sf::Event::KeyReleased &&
        event_with_mouse.event.key.code == sf::Keyboard::Space) {
        reducer.SetPanning(false);
    }

    if (event_with_mouse.event.type == sf::Event::KeyPressed &&
        event_with_mouse.event.key.code == sf::Keyboard::Space) {
        reducer.SetPanning(true);
    }

    if ((event_with_mouse.event.type == sf::Event::MouseButtonPressed &&
         event_with_mouse.event.mouseButton.button == sf::Mouse::Middle) ||
        (event_with_mouse.event.type == sf::Event::KeyReleased &&
         event_with_mouse.event.key.code == sf::Keyboard::R)) {
        reducer.SetRotation(
            (reducer.GetState().editor_state.selected_tile_rotation + 90) %
            360);
    }

    if (reducer.GetState().editor_state.panning) {
        auto mouse_delta =
            event_with_mouse.window_mouse_position - last_mouse_position;
        house_view_layer.MoveView(mouse_delta.x * -1, mouse_delta.y * -1);
        // reducer.MoveHouseView(mouse_delta.x * -1, mouse_delta.y * -1);
    }

    last_mouse_position = event_with_mouse.window_mouse_position;

    if (event_with_mouse.event.type == sf::Event::Resized) {
        house_view_layer.MoveView(event_with_mouse.event.size.width,
                                  event_with_mouse.event.size.height);

        /*
        reducer.MoveHouseView(event_with_mouse.event.size.width,
                              event_with_mouse.event.size.height);
        */
        house_render_texture.create(event_with_mouse.event.size.width,
                                    event_with_mouse.event.size.height);
    }

    HandleInputBoxSelection(event_with_mouse, reducer);
}

void EditorController::HandleInputBoxSelection(
    const EventWithMouse &event_with_mouse, HouseSceneReducer &reducer) {

    auto window_target_coords = house_render_texture.mapPixelToCoords(
        sf::Vector2i(event_with_mouse.window_mouse_position.x,
                     event_with_mouse.window_mouse_position.y));
    reducer.UpdateMousePosition(window_target_coords);

    if (event_with_mouse.event.type == sf::Event::MouseButtonPressed &&
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left) {
        reducer.MousePressedAt();
    }

    if (event_with_mouse.event.type == sf::Event::MouseButtonReleased &&
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left) {
        reducer.MouseReleased(window_target_coords, tile_map_sprite_size);
    }
}
