#include <vector>
#include "Animation.h"

Animation::Animation(const SpriteSheet& sprite_sheet, std::vector<AnimationFrame> animation_frames, int frame_width, int frame_height, int speed) 
    :   animation_frames(animation_frames), 
        current_frame_counter(0),
        frame_height(frame_height), 
        frame_width(frame_width), 
        speed(speed),
        sprite_sheet(sprite_sheet) {
    sprite.setTexture(sprite_sheet.GetTexture());
    sprite.setScale(sprite_sheet.GetScale(), sprite_sheet.GetScale());
    sprite.setOrigin(frame_width / 2, frame_height / 2);
}

void Animation::Update ()  {
    sf::IntRect current_frame_rect = sf::IntRect(
        animation_frames[current_frame_counter / speed].x * frame_width,
        animation_frames[current_frame_counter / speed].y * frame_height,
        frame_width, 
        frame_height
    );

    sprite.setTextureRect(current_frame_rect);
    if (current_frame_counter == (animation_frames.size() * speed) - 1) {
        current_frame_counter = 0;
    } else {
        current_frame_counter++;
    }
}

