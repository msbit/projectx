#include <filesystem>
#include <windows.h>

#include "AssetWatcher.h"

AssetWatcher::AssetWatcher(int scale)
    : watcher(&AssetWatcher::StartWatching, this), required_reload(false),
      shutdown(false) {}

AssetWatcher::~AssetWatcher() {
    shutdown = true;
    watcher.join();
}

void AssetWatcher::StartWatching() {
    auto asset_path = std::filesystem::absolute("assets");

    auto change_handle = FindFirstChangeNotification(
        asset_path.string().c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);

    if (change_handle == INVALID_HANDLE_VALUE) {
        exit(GetLastError());
    }

    while (!shutdown) {
        auto wait_status = WaitForSingleObject(change_handle, 5000);

        if (wait_status == WAIT_OBJECT_0) {
            Sleep(10); // TODO : I think there is a race conidition here and the
                       // file is still locked
            required_reload = true;

            if (FindNextChangeNotification(change_handle) == false) {
                exit(GetLastError());
            }
        }
    }

    FindCloseChangeNotification(change_handle);
}
