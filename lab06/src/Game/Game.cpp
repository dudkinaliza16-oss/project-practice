#include "Game.h"
#include <optional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map.h"
using namespace sf;

const int WIDTH_WINDOW = 960;
const int HEIGHT_WINDOW = 640;

void Game::Run(){
	sf::RenderWindow window(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Game");
    window.setFramerateLimit(60);

	MapInit(map);
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
		MapDraw(window, map);
        player.Draw(window);
        window.display();
    }
}