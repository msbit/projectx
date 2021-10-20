#include <optional>
#include <vector>

#include "AssetWatcher.h"
#include "BoxSelectionView.h"
#include "ControllerScheme.h"
#include "CreatedRoomSelectionView.h"
#include "EditorController.h"
#include "EntityFactory.h"
#include "EntityView.h"
#include "GamepadControllerScheme.h"
#include "GridSelectionView.h"
#include "GridView.h"
#include "HouseSceneFactory.h"
#include "KeyboardControllerScheme.h"
#include "PlayerController.h"
#include "SelectedTileView.h"
#include "ShadowView.h"
#include "SpriteSheet.h"
#include "TileBackgroundView.h"
#include "TilePaletteView.h"
#include "ToolbarController.h"
#include "ToolbarToolsView.h"

std::unique_ptr<HouseScene>
HouseSceneFactory::Init(const int screen_height, const int screen_width,
                        const std::optional<std::string> map_path_optional) {
    auto tile_map_sprite_size = 16;

    auto screen = InitScreen(screen_width, screen_height, tile_map_sprite_size);
    auto asset_watcher =
        InitAssetWatcher(screen->GetScale(), tile_map_sprite_size, screen);
    auto reducer = std::make_shared<HouseSceneReducer>();

    // Create Tile Sprites
    auto tile_sprites = asset_watcher->GetSpriteSheet("tile_map")->GetSprites();

    std::for_each(
        tile_sprites.begin(), tile_sprites.end(),
        [&asset_watcher, &reducer](const auto &sprite) {
            reducer->AddTilePaletteTile(
                TilePaletteTile{sprite, TilePaletteType::PaletteTile},
                asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize());
        });

    // TODO : This method might need to move out of here at some point
    asset_watcher->GetSpriteSheet("entity_map")
        ->CreateIconSprite(sf::Color::Red, 0);
    asset_watcher->GetSpriteSheet("entity_map")
        ->CreateIconSprite(sf::Color::Green, 1);
    asset_watcher->GetSpriteSheet("toolbar_sprite_sheet")
        ->CreateIconSprite(sf::Color::Yellow, 0);

    auto entity_sprites =
        asset_watcher->GetSpriteSheet("entity_map")->GetSprites();
    reducer->AddTilePaletteTile(
        TilePaletteTile{entity_sprites[0], TilePaletteType::PaletteEntity,
                        EntityType::GhostEntity},
        asset_watcher->GetSpriteSheet("entity_map")->GetSpriteSize());

    reducer->AddTilePaletteTile(
        TilePaletteTile{entity_sprites[1], TilePaletteType::PaletteEntity,
                        EntityType::DoorEntity},
        asset_watcher->GetSpriteSheet("entity_map")->GetSpriteSize());

    /*

    TODO : Figure out where this code lives


    int total_height =
        (state.editor_state.tile_palette_tiles.size() *
         (asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize() +
          screen.GetTilePaletteOffset())) +
        screen.GetTilePaletteOffset();

    state.editor_state.tile_palette_background = sf::RectangleShape(
        sf::Vector2f(screen.GetLeftToolbarWidth(), total_height));

    state.editor_state.tile_palette_background.setFillColor(
        sf::Color(60, 60, 60, 255));


    state.editor_state.tile_palette_view = sf::View(sf::FloatRect(
        0, 0, screen.GetLeftToolbarWidth(), screen.GetWindowSize().height));
    state.editor_state.toolbar_view =
        sf::View(sf::FloatRect(0, 0, screen.GetWindowSize().width,
    screen.GetToolbarOffset())); state.house_view = sf::View(sf::FloatRect(0, 0,
    screen.GetWindowSize().width, screen.GetWindowSize().height));

    */

    auto map =
        InitMap(map_path_optional, *reducer, screen, tile_map_sprite_size);

    auto animations = InitAnimations(asset_watcher, screen);
    auto view_layers = InitViewLayers(asset_watcher, screen, animations,
                                      map->GetBounds(), *map);

    auto controller_list =
        InitControllers(asset_watcher, animations, screen, map, view_layers);

    reducer->InitSelectionRectangle(
        asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize());
    reducer->AddEntity(EntityFactory::Player(sf::Vector2f(0, 0)));

    reducer->AddTool(Tool{ToolType::Room, 0});

    return std::make_unique<HouseScene>(asset_watcher, controller_list,
                                        view_layers, animations, reducer);
}

std::shared_ptr<Map> HouseSceneFactory::InitMap(
    const std::optional<std::string> map_path, HouseSceneReducer &reducer,
    std::shared_ptr<Screen> screen, int tile_map_sprite_size) {
    if (map_path.has_value()) {
        return std::make_shared<Map>(reducer, map_path.value(), screen,
                                     tile_map_sprite_size);
    } else {
        return std::make_shared<Map>(reducer, screen, tile_map_sprite_size,
                                     sf::IntRect(0, 0, 20, 20));
    }
}

std::shared_ptr<ControllerScheme> HouseSceneFactory::InitControllerScheme() {
    if (sf::Joystick::isConnected(0)) {
        return std::make_shared<GamepadControllerScheme>();
    } else {
        return std::make_shared<KeyboardControllerScheme>();
    }
}

std::vector<std::shared_ptr<TimedController>>
HouseSceneFactory::InitControllers(std::shared_ptr<AssetWatcher> asset_watcher,
                                   AnimationMap animations,
                                   std::shared_ptr<Screen> screen,
                                   std::shared_ptr<Map> map,
                                   ViewLayerMap view_layer_map) {
    auto controller_list = std::vector<std::shared_ptr<TimedController>>();
    auto controller_scheme = InitControllerScheme();

    controller_list.push_back(
        std::make_shared<TimedController>(std::make_unique<EditorController>(
            asset_watcher->GetSpriteSheet("entity_map")->GetSpriteSize(),
            view_layer_map["tile_palette"]->GetRenderTexture(),
            view_layer_map["house_map"]->GetRenderTexture(), *map, screen,
            *view_layer_map["house_map"], *view_layer_map["tile_palette"])));

    controller_list.push_back(std::make_shared<TimedController>(
        std::make_unique<ToolbarController>(screen)));

    controller_list.push_back(
        std::make_shared<TimedController>(std::make_unique<PlayerController>(
            animations, controller_scheme, map, *view_layer_map["house_map"])));

    return controller_list;
}

std::shared_ptr<AssetWatcher>
HouseSceneFactory::InitAssetWatcher(int scale, int tile_map_sprite_size,
                                    std::shared_ptr<Screen> screen) {
    auto asset_watcher = std::make_shared<AssetWatcher>(scale);

    asset_watcher->RegisterSpriteSheet(
        "tile_map",
        std::make_shared<SpriteSheet>("./assets/house.png",
                                      tile_map_sprite_size, 20, 20, screen));

    asset_watcher->RegisterSpriteSheet(
        "entity_map", std::make_shared<SpriteSheet>(16, screen));

    asset_watcher->RegisterSpriteSheet(
        "player_sprite_sheet",
        std::make_shared<SpriteSheet>("./assets/NightThief.png", 320, 1, 1,
                                      screen));

    asset_watcher->RegisterSpriteSheet(
        "toolbar_sprite_sheet", std::make_shared<SpriteSheet>(8, screen));

    return asset_watcher;
}

std::shared_ptr<Screen>
HouseSceneFactory::InitScreen(int window_width, int window_height,
                              int tile_map_sprite_size) {
    int scale = 4;
    int toolbar_icon_size = 15;
    int toolbar_icon_padding = 10;
    int tile_palette_offset = 20;
    int toolbar_offset = 60;

    return std::make_shared<Screen>(
        sf::IntRect(0, 0, window_width, window_height), scale,
        toolbar_icon_padding, toolbar_icon_size, tile_palette_offset,
        toolbar_offset, tile_map_sprite_size);
}

std::shared_ptr<std::unordered_map<EntityState, Animation>>
HouseSceneFactory::InitAnimations(std::shared_ptr<AssetWatcher> asset_watcher,
                                  std::shared_ptr<Screen> screen) {
    std::shared_ptr<std::unordered_map<EntityState, Animation>>
        player_animations =
            std::make_shared<std::unordered_map<EntityState, Animation>>();

    std::vector<AnimationFrame> idle_frames;
    for (auto col = 0; col < 10; col++) {
        idle_frames.push_back(AnimationFrame{col, 0});
    }

    player_animations->insert(
        {EntityState::Idle,
         Animation(asset_watcher->GetSpriteSheet("player_sprite_sheet"),
                   idle_frames, 32, 32, 8, screen)});

    std::vector<AnimationFrame> throw_frames;
    for (auto col = 0; col < 10; col++) {
        throw_frames.push_back(AnimationFrame{col, 1});
    }

    player_animations->insert(
        {EntityState::Throwing,
         Animation(asset_watcher->GetSpriteSheet("player_sprite_sheet"),
                   throw_frames, 32, 32, 8, screen)});

    std::vector<AnimationFrame> walk_frames;
    for (auto col = 0; col < 10; col++) {
        walk_frames.push_back(AnimationFrame{col, 2});
    }

    player_animations->insert(
        {EntityState::Walking,
         Animation(asset_watcher->GetSpriteSheet("player_sprite_sheet"),
                   walk_frames, 32, 32, 8, screen)});

    std::vector<AnimationFrame> attack_frames;
    for (auto col = 0; col < 10; col++) {
        attack_frames.push_back(AnimationFrame{col, 3});
    }

    player_animations->insert(
        {EntityState::Attacking,
         Animation(asset_watcher->GetSpriteSheet("player_sprite_sheet"),
                   attack_frames, 32, 32, 8, screen)});

    std::vector<AnimationFrame> die_frames;
    for (auto col = 0; col < 10; col++) {
        die_frames.push_back(AnimationFrame{col, 4});
    }

    player_animations->insert(
        {EntityState::Dying,
         Animation(asset_watcher->GetSpriteSheet("player_sprite_sheet"),
                   die_frames, 32, 32, 8, screen)});

    return player_animations;
}

ViewLayerMap HouseSceneFactory::InitViewLayers(
    std::shared_ptr<AssetWatcher> asset_watcher, std::shared_ptr<Screen> screen,
    AnimationMap player_animations, sf::IntRect map_bounds, Map &map) {

    auto view_layer_map = ViewLayerMap();

    auto house_map_texture_size = screen->GetWindowSize();
    sf::View house_map_view = sf::View(sf::FloatRect(
        0, 0, screen->GetWindowSize().width, screen->GetWindowSize().height));
    view_layer_map["house_map"] =
        std::make_shared<ViewLayer>(house_map_texture_size, house_map_view);

    view_layer_map["house_map"]->AddView(std::make_unique<SelectedTileView>(
        asset_watcher->GetSpriteSheet("tile_map")));

    view_layer_map["house_map"]->AddView(std::make_unique<TileBackgroundView>(
        asset_watcher->GetSpriteSheet("tile_map")));

    view_layer_map["house_map"]->AddView(std::make_unique<GridView>(
        asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize(), map));

    view_layer_map["house_map"]->AddView(std::make_unique<EntityView>(
        asset_watcher->GetSpriteSheet("entity_map"), player_animations));

    view_layer_map["house_map"]->AddView(std::make_unique<BoxSelectionView>());
    view_layer_map["house_map"]->AddView(
        std::make_unique<CreatedRoomSelectionView>());

    view_layer_map["house_map"]->AddView(std::make_unique<GridSelectionView>(
        asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize()));

    view_layer_map["house_map"]->AddView(std::make_unique<ShadowView>(
        sf::IntRect(0, 0,
                    asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize() *
                        map_bounds.width,
                    asset_watcher->GetSpriteSheet("tile_map")->GetSpriteSize() *
                        map_bounds.height)));

    auto tile_palette_texture_size = screen->GetTilePaletteArea();
    sf::View tile_palette_view =
        sf::View(sf::FloatRect(0, 0, screen->GetTilePaletteArea().width,
                               screen->GetWindowSize().height));
    view_layer_map["tile_palette"] = std::make_shared<ViewLayer>(
        tile_palette_texture_size, tile_palette_view);
    view_layer_map["tile_palette"]->AddView(std::make_unique<TilePaletteView>(
        screen->GetWindowSize().height, screen->GetTilePaletteArea().width,
        screen));

    auto toolbar_texture_size = screen->GetToolbarArea();
    sf::View toolbar_view = sf::View(sf::FloatRect(
        0, 0, screen->GetWindowSize().width, screen->GetToolbarOffset()));
    view_layer_map["toolbar"] =
        std::make_shared<ViewLayer>(toolbar_texture_size, toolbar_view);
    view_layer_map["toolbar"]->AddView(std::make_unique<ToolbarToolsView>(
        asset_watcher->GetSpriteSheet("toolbar_sprite_sheet"), screen));

    return view_layer_map;
}
