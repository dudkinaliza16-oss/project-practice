#include <optional>
#include <SFML/Graphics.hpp>
int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "window by Dudkina EM");
    window.setFramerateLimit(60);
    sf::Color bgColor = sf::Color::Black;
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    if (bgColor == sf::Color::Black) {
                        bgColor = sf::Color::White;
                    } else {
                        bgColor = sf::Color::Black;
                    }
                }
            }
        }
        window.clear(bgColor);
        window.display();
    }
    return 0;
}
