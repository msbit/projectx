#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.h"
#include "HouseSceneState.h"
#include "TileLayer.h"
#include "TilePaletteTile.h"

class HouseSceneReducer {
    HouseSceneState state;

  public:
    const HouseSceneState &GetState();
    void ToggleEditorEnabled();
    void ResetPlayer();
    void UpdateSelectedTileIndex(int);
    void AddTileLayer(TileLayer);
    void AddEntity(Entity);
    void AddTile(int, int);
    void AddEntity(int, int);
    void SetPanning(bool);
    void SetRotation(int);
    void AddTilePaletteTile(TilePaletteTile, int);
    void InitSelectionRectangle(int);
    void SetSelectionRectanglePosition(sf::Vector2f);
    void UpdateSelectedEditorSquare(sf::Vector2i);
    void SetEntityTransform(sf::Vector2f);
    void SetEntityVelocity(sf::Vector2f);
    void MousePressedAt();
    void MouseReleased(sf::Vector2f, int);
    void UpdateMousePosition(sf::Vector2f);
    void AddTool(Tool);
    void SetPlayerState(EntityState);
    void SetPlayerDirection(sf::Vector2f);
    void AddRoom(sf::IntRect);
    void ClearEditorSelection();
};
