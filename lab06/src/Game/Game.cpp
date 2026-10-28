#include "Game.h"
#include <optional>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Map.h"
using namespace sf;

const int WIDTH_WINDOW = 960;
const int HEIGHT_WINDOW = 640;

void Game::Run(){
	sf::RenderWindow window(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Game");
    window.setFramerateLimit(60);

    view.setSize({WIDTH_WINDOW, HEIGHT_WINDOW});

	MapInit(map);
    player.Init();

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        player.Update(dt, map);

        sf::Vector2f targetCenter = player.sprite.getPosition();

        float minX = WIDTH_WINDOW / 2.0f;
        float maxX = (map.width * WORLD_TILE) - (WIDTH_WINDOW / 2.0f);
        float minY = HEIGHT_WINDOW / 2.0f;
        float maxY = (map.height * WORLD_TILE) - (HEIGHT_WINDOW / 2.0f);

        float clampedX = std::clamp(targetCenter.x, minX, maxX);
        float clampedY = std::clamp(targetCenter.y, minY, maxY);
        view.setCenter({clampedX, clampedY});

        window.clear(sf::Color(62, 95, 138));
        window.setView(view);
        MapDraw(window, map);
        player.Draw(window);
        window.display();
    }
}