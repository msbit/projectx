#include <filesystem>
#include <iostream>

#include "AssetWatcher.h"

std::shared_ptr<SpriteSheet>
AssetWatcher::GetSpriteSheet(std::string sprite_sheet_name) const {
    return sprite_sheets.at(sprite_sheet_name);
}

void AssetWatcher::Reload() {
    for (auto const &sprite_sheet : sprite_sheets) {
        sprite_sheet.second->Reload();
    }
}

void AssetWatcher::ReloadIfRequired() {
    if (required_reload) {
        Reload();
        required_reload = false;
    }
}

void AssetWatcher::RegisterSpriteSheet(
    std::string sprite_sheet_name, std::shared_ptr<SpriteSheet> sprite_sheet) {
    sprite_sheets[sprite_sheet_name] = sprite_sheet;
}
