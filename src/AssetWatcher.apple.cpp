#include <fcntl.h>
#include <sys/errno.h>
#include <sys/event.h>

#include "AssetWatcher.h"

AssetWatcher::AssetWatcher(int scale)
    : watcher(&AssetWatcher::StartWatching, this), required_reload(false),
      shutdown(false) {}

AssetWatcher::~AssetWatcher() {
    shutdown = true;
    watcher.join();
}

void AssetWatcher::StartWatching() {
    auto queue = kqueue();
    if (queue == -1) {
        exit(errno);
    }

    auto fd = open("assets", O_RDONLY);
    if (fd == -1) {
        exit(errno);
    }

    struct kevent change;
    EV_SET(&change, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR | EV_ENABLE, NOTE_WRITE,
           0, nullptr);

    if (kevent(queue, &change, 1, nullptr, 0, nullptr) == -1) {
        exit(errno);
    }

    struct timespec wait = {5, 0};
    while (!shutdown) {
        struct kevent event = {0};
        auto result = kevent(queue, nullptr, 0, &event, 1, &wait);
        if (result == -1) {
            exit(errno);
        }

        if (result == 1 && event.ident == fd) {
            required_reload = true;
        }
    }
}
