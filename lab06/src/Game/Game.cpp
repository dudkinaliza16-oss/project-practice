#include "Game.h"
#include <optional>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Map.h"
using namespace sf;

const int WIDTH_WINDOW = 960;
const int HEIGHT_WINDOW = 640;

void Game::Run() {
    window.create(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Game");
    window.setFramerateLimit(60);

    view.setSize({WIDTH_WINDOW, HEIGHT_WINDOW});
    MapInit(map);
    player.Init();

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::update(float dt) {
    switch (gameState) {
        case GameState::Playing:
			updatePlaying(dt);
			break;
        default:
			break;
    }
}

void Game::updatePlaying(float dt) {
    player.Update(dt, map);

    sf::Vector2f target = player.sprite.getPosition();
    float minX = WIDTH_WINDOW / 2.0f;
    float maxX = map.width * WORLD_TILE - WIDTH_WINDOW / 2.0f;
    float minY = HEIGHT_WINDOW / 2.0f;
    float maxY = map.height * WORLD_TILE - HEIGHT_WINDOW / 2.0f;
    view.setCenter({std::clamp(target.x, minX, maxX), std::clamp(target.y, minY, maxY)});
}

void Game::render() {
    window.clear(sf::Color(62, 95, 138));
    switch (gameState) {
        case GameState::Menu:
			renderMenu();
			break;
        case GameState::Playing:
			renderPlaying();
			break;
        default:
			break;
    }
    window.display();
}

void Game::renderPlaying() {
    window.setView(view);
    MapDraw(window, map);
    player.Draw(window);
}

void Game::renderMenu() {
    window.clear(sf::Color(62, 95, 138));
}

void Game::processEvents() {
	while (const std::optional<sf::Event> event = window.pollEvent()){
		if (event->is<sf::Event::Closed>()) {
        	window.close();
        }
		switch (gameState) {
        	case GameState::Menu:
            	handleMenuInput(*event);
			    break;
            case GameState::Playing:
                handlePlayingInput(*event);
				break;
		    //case GameState::Paused:
            //    handlePausedInput(event);   break;
            //case GameState::GameOver:
            // handleGameOverInput(event); break;
        }
   }
}
void Game::handleMenuInput(const sf::Event& event) {
    const sf::Event::KeyPressed* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) return;
	if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space) {
		restartGame();
        gameState = GameState::Playing;
	}
}

void Game::handlePlayingInput(const sf::Event &event) {
	const sf::Event::KeyPressed* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) return;
	if (key->code == sf::Keyboard::Key::Enter) {
		restartGame();
        gameState = GameState::Playing;
	}
}

void Game::restartGame() {
    player.Init();
}