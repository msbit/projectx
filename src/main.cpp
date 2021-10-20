#include <optional>
#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "EventWithMouse.h"
#include "HouseSceneFactory.h"

int main(int argc, char **argv) {
    int window_width = 1920;
    int window_height = 1080;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                            "SFML works!");
    window.setFramerateLimit(60);

    std::optional map_path_optional =
        argc == 2 ? std::optional<std::string>{argv[1]} : std::nullopt;

    auto current_scene =
        HouseSceneFactory::Init(window_height, window_width, map_path_optional);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width,
                                          event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            current_scene->HandleInput(
                EventWithMouse{event, sf::Mouse::getPosition(window)});
        }
        current_scene->Update();
        window.clear();
        current_scene->Draw(window);
        window.display();
    }

    return 0;
}
