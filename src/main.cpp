#include "ScreenValues.h"
#include "HouseSceneFactory.h"
#include <SFML/Graphics.hpp>
#include <optional>

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                            "SFML works!");
    window.setFramerateLimit(60);

    auto map_path_optional = [&]() -> std::optional<std::string> {
        if (argc == 2)
            return {argv[1]};

        return {};
    }();

    auto current_scene = HouseSceneFactory::Init(
        window_height, 
        window_width,
        map_path_optional
    );

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
