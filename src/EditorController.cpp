#include "EditorController.h"

EditorController::EditorController(
    int tile_map_sprite_size, 
    sf::RenderTexture& tile_palette_render_texture,
    sf::RenderTexture& house_render_texture,
    Map& map
) :
tile_map_sprite_size(tile_map_sprite_size),
tile_palette_render_texture(tile_palette_render_texture),
house_render_texture(house_render_texture),
map(map) {}

void EditorController::Update (HouseSceneReducer& reducer) {
    auto state = reducer.GetState();

    auto selected_tile_position = state
        .editor_state
        .tile_palette_tiles[state.editor_state.selected_tile_index]
        .icon.getPosition();

    reducer.SetSelectionRectanglePosition(selected_tile_position);
}

void EditorController::HandleInput (EventWithMouse event_with_mouse, HouseSceneReducer& reducer) {
 
    if (event_with_mouse.event.type == sf::Event::KeyPressed && 
        event_with_mouse.event.key.code == sf::Keyboard::E) {
        reducer.ToggleEditorEnabled();
        reducer.ResetPlayer();
        return;
    }

    if (!reducer.GetState().editor_state.editor_enabled) {
        return;
    }

    if (event_with_mouse.event.type == sf::Event::MouseButtonPressed && 
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left) {

        // Manage Selection Changed
        auto event_target_coords = tile_palette_render_texture.mapPixelToCoords(
            sf::Vector2i(
                event_with_mouse.event.mouseButton.x, 
                event_with_mouse.event.mouseButton.y
            )
        );

        auto found = std::find_if(
            reducer.GetState().editor_state.tile_palette_tiles.begin(), 
            reducer.GetState().editor_state.tile_palette_tiles.end(), 
            [event_target_coords](const auto &t) {
                return t.icon.getGlobalBounds().contains(event_target_coords);
            }
        );

        if (found != reducer.GetState().editor_state.tile_palette_tiles.end()) {
            reducer.UpdateSelectedTileIndex(
                found - reducer.GetState().editor_state.tile_palette_tiles.begin()
            );
        }
    }

    if (event_with_mouse.event.type == sf::Event::MouseWheelMoved && 
        event_with_mouse.event.mouseButton.x < reducer.GetState().editor_state.tile_palette_bounds.getSize().x
    ) {
        int upper_scroll_center = reducer.GetState().editor_state.tile_palette_bounds.getSize().y / 2;

        int lower_scroll_center = 
            reducer.GetState().editor_state.tile_palette_background_total_height - 
            upper_scroll_center;

        if (event_with_mouse.event.mouseWheel.delta < 0 && 
            reducer.GetState().editor_state.tile_palette_view.getCenter().y > upper_scroll_center
        ) {
            reducer.MoveTilePaletteView(0, 100 * event_with_mouse.event.mouseWheel.delta);
        }

        if (event_with_mouse.event.mouseWheel.delta > 0 && 
            reducer.GetState().editor_state.tile_palette_view.getCenter().y < lower_scroll_center
        ) {
            reducer.MoveTilePaletteView(0, 100 * event_with_mouse.event.mouseWheel.delta);
        }
    }

    // TODO : Figure out why we are donig this
    //auto current_target_coords = house_render_texture.mapPixelToCoords(
        //event_with_mouse.window_mouse_position
    //);

    // current_mouse_grid_position.x = floor(current_target_coords.x / tile_map.GetSpriteSize());
    // current_mouse_grid_position.y = floor(current_target_coords.y / tile_map.GetSpriteSize());
     
    if (event_with_mouse.event.type == sf::Event::MouseButtonPressed && 
        event_with_mouse.event.mouseButton.button == sf::Mouse::Left) {
        
        sf::Vector2f event_target_coords = house_render_texture.mapPixelToCoords(
            sf::Vector2i(
                event_with_mouse.event.mouseButton.x, 
                event_with_mouse.event.mouseButton.y
            )
        );
        
        sf::IntRect pixel_bounds(
            0, 
            0,
            reducer.GetState().map_bounds.width * tile_map_sprite_size,
            reducer.GetState().map_bounds.height * tile_map_sprite_size
        );

        if (pixel_bounds.contains(event_target_coords.x, event_target_coords.y) && 
            !reducer.GetState().editor_state.tile_palette_background.getGlobalBounds()
                .contains(event_with_mouse.event.mouseButton.x, event_with_mouse.event.mouseButton.y)
        ) {
            auto x = (int)event_target_coords.x / tile_map_sprite_size;
            auto y = (int)event_target_coords.y / tile_map_sprite_size;

            auto selected_tile = reducer.GetState().editor_state.tile_palette_tiles[
                reducer.GetState().editor_state.selected_tile_index
            ];

            if (selected_tile.type == PaletteTile) {
                reducer.AddTile(x, y);
            } else if (selected_tile.type == PaletteEntity){
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
        reducer.SetRotation((reducer.GetState().editor_state.selected_tile_rotation + 90) % 360);
    }

    if (reducer.GetState().editor_state.panning) {
        auto mouse_delta = current_mouse_position - last_mouse_position;
        reducer.MoveHouseView(mouse_delta.x * -1, mouse_delta.y * -1);
    }

    last_mouse_position = current_mouse_position;

    if (event_with_mouse.event.type == sf::Event::Resized) {
        reducer.MoveHouseView(
            event_with_mouse.event.size.width, 
            event_with_mouse.event.size.height
        );
        house_render_texture.create(
            event_with_mouse.event.size.width, 
            event_with_mouse.event.size.height
        ); 
    }
 
}
