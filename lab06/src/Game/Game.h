#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Player.h"
#include "Map.h"
#include "GameState.h"
#include "MenuScreen.h"
#include "Hud.h"

constexpr int WIDTH_WINDOW = 960;
constexpr int HEIGHT_WINDOW = 640;

struct Game {
    sf::RenderWindow window;
    sf::Clock clock;
    sf::View view;
    Map map;
    Player player;
    Hud hud;

    GameState gameState{GameState::Menu};
    sf::Font font;

    MenuScreen mainMenu;
    MenuScreen pauseMenu;
    MenuScreen gameOverMenu;
    MenuScreen victoryMenu;

    void Run();
    void InitUI();
    void Update(float dt);
    void UpdatePlaying(float dt);
    void Render();
    void ProcessEvents();

    void HandlePlayingInput(const sf::Event& event);

    void RestartGame();
};