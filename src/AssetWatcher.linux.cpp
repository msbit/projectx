#include <errno.h>
#include <poll.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "AssetWatcher.h"

AssetWatcher::AssetWatcher(int scale)
    : watcher(&AssetWatcher::StartWatching, this), required_reload(false),
      shutdown(false) {}

AssetWatcher::~AssetWatcher() {
    shutdown = true;
    watcher.join();
}

void AssetWatcher::StartWatching() {
    auto fd = inotify_init();
    if (fd == -1) {
        exit(errno);
    }

    auto wd = inotify_add_watch(
        fd, "assets", IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE | IN_ONLYDIR);
    if (wd == -1) {
        exit(errno);
    }

    char buf[1024];

    struct pollfd pfd = {fd, POLLIN, 0};
    while (!shutdown) {
        auto result = poll(&pfd, 1, 5000);
        if (result == -1) {
            exit(errno);
        }

        if (result == 0) {
            continue;
        }

        auto count = read(fd, buf, 1024);
        if (count <= 0) {
            exit(errno);
        }

        for (auto p = buf; p < buf + count;) {
            auto event = (struct inotify_event *)p;
            p += sizeof(struct inotify_event) + event->len;
            if (event->wd == wd) {
                required_reload = true;
            }
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
