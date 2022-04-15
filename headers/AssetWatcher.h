#pragma once

#include <atomic>
#include <map>
#include <string>
#include <thread>

#include "SpriteSheet.h"

class AssetWatcher {
    std::thread watcher;
    void StartWatching();
    std::map<std::string, std::shared_ptr<SpriteSheet>> sprite_sheets;
    void Reload();
    std::atomic<bool> required_reload;
    std::atomic<bool> shutdown;

#ifdef APPLE
    int queue;
#endif

#ifdef LINUX
    int signal;
#endif

  public:
    void ReloadIfRequired();
    std::shared_ptr<SpriteSheet> GetSpriteSheet(std::string) const;
    void RegisterSpriteSheet(std::string, std::shared_ptr<SpriteSheet>);
    AssetWatcher(int);
    ~AssetWatcher();
};