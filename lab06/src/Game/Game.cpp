#include "Game.h"
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Map.h"

void Game::Run() {
    window.create(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Game");
    window.setFramerateLimit(60);

    InitUI();
	hud.Init();
    view.setSize({WIDTH_WINDOW, HEIGHT_WINDOW});
    if (!MapInit(map, "../assets/level1.txt")) {
    	std::cerr << "Critical Error: Failed to load level 1!" << std::endl;
    }
    player.Init();
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        ProcessEvents();
        Update(dt);
        Render();
    }
}

void Game::InitUI() {
    if (!font.openFromFile("../assets/LevTypeHryvnia-Regular.ttf")) {
        std::cerr << "Ошибка: не удалось загрузить шрифт!" << std::endl;
    }

    mainMenu.Init(font, "MY GAME", {"Play", "Exit"}, 200.f);
    pauseMenu.Init(font, "PAUSE", {"Continue", "Restart", "Main Menu"}, 200.f);
    gameOverMenu.Init(font, "GAME OVER", {"Restart", "Main Menu"}, 200.f);
    victoryMenu.Init(font, "VICTORY", {"Main Menu"}, 200.f);
}

void Game::Update(float dt) {
    switch (gameState) {
        case GameState::Playing:
            UpdatePlaying(dt);
            break;
        default:
            break;
    }
}

void Game::UpdatePlaying(float dt) {
    player.Update(dt, map);
	hud.Update(player.sprite.getPosition().x, map.width * WORLD_TILE);

    if (player.state == PlayerState::Dead) {
        gameState = GameState::GameOver;
        gameOverMenu.selectedIndex = 0;
        return;
    }
	if (player.reachedVictory) {
        gameState = GameState::Victory;
        victoryMenu.selectedIndex = 0;
        return;
    }

    sf::Vector2f target = player.sprite.getPosition();
    float minX = WIDTH_WINDOW / 2.0f;
    float maxX = map.width * WORLD_TILE - WIDTH_WINDOW / 2.0f;
    float minY = HEIGHT_WINDOW / 2.0f;
    float maxY = map.height * WORLD_TILE - HEIGHT_WINDOW / 2.0f;
    view.setCenter({std::clamp(target.x, minX, maxX), std::clamp(target.y, minY, maxY)});
}

void Game::Render() {
    window.clear(sf::Color(62, 95, 138));
    switch (gameState) {
        case GameState::Menu:
            mainMenu.Draw(window, sf::Color(62, 95, 138));
            break;
        case GameState::Playing:
            window.setView(view);
            MapDraw(window, map);
            player.Draw(window);
			window.setView(window.getDefaultView());
			hud.Draw(window);
            break;
        case GameState::Paused:
            pauseMenu.Draw(window, sf::Color(40, 50, 70));
            break;
        case GameState::GameOver:
            gameOverMenu.Draw(window, sf::Color(100, 0, 0));
            break;
        case GameState::Victory:
            victoryMenu.Draw(window, sf::Color(0, 100, 0));
            break;
        default:
            break;
    }
    window.display();
}

void Game::ProcessEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        switch (gameState) {
            case GameState::Menu:
                if (mainMenu.HandleNavigation(*event)) {
                    if (mainMenu.selectedIndex == 0) {
    			        RestartGame();
        			    gameState = GameState::Playing;
        			} else if (mainMenu.selectedIndex == 1) {
        			    window.close();
        			}
                }
                break;
            case GameState::Playing:
                HandlePlayingInput(*event);
                break;
            case GameState::Paused:
                if (pauseMenu.HandleNavigation(*event)) {
                    if (pauseMenu.selectedIndex == 0) {
                        gameState = GameState::Playing;
                    } else if (pauseMenu.selectedIndex == 1) {
                        RestartGame();
                        gameState = GameState::Playing;
                    } else if (pauseMenu.selectedIndex == 2) {
                        gameState = GameState::Menu;
                        mainMenu.selectedIndex = 0;
                    }
                }
                break;
            case GameState::GameOver:
                if (gameOverMenu.HandleNavigation(*event)) {
                    if (gameOverMenu.selectedIndex == 0) {
                        RestartGame();
                        gameState = GameState::Playing;
                    } else if (gameOverMenu.selectedIndex == 1) {
                        gameState = GameState::Menu;
                        mainMenu.selectedIndex = 0;
                    }
                }
                break;
            case GameState::Victory: {
                if (victoryMenu.HandleNavigation(*event)) {
                    if (victoryMenu.selectedIndex == 0) {
    			        gameState = GameState::Menu;
    			        mainMenu.selectedIndex = 0;
       				}
                }
                break;
            }
        }
    }
}

void Game::HandlePlayingInput(const sf::Event& event) {
    const sf::Event::KeyPressed* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) {
		return;
	}
    if (key->code == sf::Keyboard::Key::Escape) {
        gameState = GameState::Paused;
        pauseMenu.selectedIndex = 0;
    }
}

void Game::RestartGame() {
    player.Init();
}