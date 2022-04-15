#include <errno.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "AssetWatcher.h"

AssetWatcher::AssetWatcher(int scale)
    : required_reload(false), shutdown(false) {
    if ((signal = eventfd(0, 0)) == -1) {
        exit(errno);
    }

    watcher = std::thread(&AssetWatcher::StartWatching, this);
}

AssetWatcher::~AssetWatcher() {
    shutdown = true;
    write(signal, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
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

    struct pollfd pfds[] = {{fd, POLLIN, 0}, {signal, POLLIN, 0}};
    while (!shutdown) {
        auto result = poll(pfds, 2, 5000);
        if (result == -1) {
            exit(errno);
        }

        if (result == 0) {
            continue;
        }

        if ((pfds[0].revents & POLLIN) == POLLIN) {
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

        if ((pfds[1].revents & POLLIN) == POLLIN) {
            break;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    close(signal);
}
