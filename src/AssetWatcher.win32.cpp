#include <filesystem>
#include <windows.h>

#include "AssetWatcher.h"

AssetWatcher::AssetWatcher(int scale)
    : required_reload(false), shutdown(false) {
    if ((signal = CreateEvent(nullptr, FALSE, FALSE, nullptr)) == nullptr) {
        exit(GetLastError());
    }

    watcher = std::thread(&AssetWatcher::StartWatching, this);
}

AssetWatcher::~AssetWatcher() {
    shutdown = true;
    SetEvent(signal);
    watcher.join();
}

void AssetWatcher::StartWatching() {
    auto asset_path = std::filesystem::absolute("assets");

    auto change_handle = FindFirstChangeNotification(
        asset_path.string().c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);

    if (change_handle == INVALID_HANDLE_VALUE) {
        exit(GetLastError());
    }

    HANDLE handles[] = {change_handle, signal};

    while (!shutdown) {
        auto wait_status = WaitForMultipleObjects(2, handles, FALSE, 5000);

        if (wait_status == WAIT_OBJECT_0) {
            Sleep(10); // TODO : I think there is a race conidition here and the
                       // file is still locked
            required_reload = true;

            if (FindNextChangeNotification(change_handle) == false) {
                exit(GetLastError());
            }
        }

        if (wait_status == (WAIT_OBJECT_0 + 1)) {
            break;
        }
    }

    FindCloseChangeNotification(change_handle);
}
