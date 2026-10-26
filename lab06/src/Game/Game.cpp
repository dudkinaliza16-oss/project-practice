#include "Game.h"
#include <optional>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

const int WIDTH_WINDOW = 800;
const int HEIGHT_WINDOW = 600;

void Game::Run(){
	sf::RenderWindow window(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Game");
    window.setFramerateLimit(60);

    player.Init();

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        player.Update(dt);

       window.clear(sf::Color(62, 95, 138));
        player.Draw(window);
        window.display();
    }
}