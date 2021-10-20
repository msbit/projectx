#pragma once

#include <memory>

#include "Screen.h"
#include "SpriteSheet.h"

struct AnimationFrame {
    int x;
    int y;
};

class Animation {
    std::vector<AnimationFrame> animation_frames;
    std::size_t current_frame_counter;
    int frame_height;
    int frame_width;
    int speed;
    const std::shared_ptr<SpriteSheet> sprite_sheet;
    const std::shared_ptr<Screen> screen;

  public:
    sf::Sprite sprite;
    Animation(const std::shared_ptr<SpriteSheet>, std::vector<AnimationFrame>,
              int, int, int, const std::shared_ptr<Screen>);
    void Update();
};
