#include <fcntl.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <unistd.h>

#include "AssetWatcher.h"

AssetWatcher::AssetWatcher(int scale)
    : required_reload(false), shutdown(false) {
    if ((queue = kqueue()) == -1) {
        exit(errno);
    }

    struct kevent change;
    EV_SET(&change, 42, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, nullptr);
    if (kevent(queue, &change, 1, nullptr, 0, nullptr) == -1) {
        exit(errno);
    }

    watcher = std::thread(&AssetWatcher::StartWatching, this);
}

AssetWatcher::~AssetWatcher() {
    shutdown = true;
    struct kevent change;
    EV_SET(&change, 42, EVFILT_USER, 0, NOTE_TRIGGER, 0, nullptr);
    kevent(queue, &change, 1, nullptr, 0, nullptr);
    watcher.join();
}

void AssetWatcher::StartWatching() {
    auto fd = open("assets", O_RDONLY);
    if (fd == -1) {
        exit(errno);
    }

    struct kevent change;
    EV_SET(&change, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_WRITE, 0,
           nullptr);

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

        if (result == 0) {
            continue;
        }

        if (event.ident == fd) {
            required_reload = true;
        }

        if (event.filter == EVFILT_USER && event.ident == 42) {
            break;
        }
    }
}
