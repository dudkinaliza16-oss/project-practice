#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "Player.h"
#include "Map.h"

enum class GameState : uint8_t {
    Menu,
    Playing,
    Paused,
    GameOver
};

enum class GameResult : uint8_t {
    Process,
    Victory,
    Defeat
};

struct Game {
    void Run();

    void processEvents();
    void update(float dt);
    void render();

    void handleMenuInput(const sf::Event& event);
    void handlePlayingInput(const sf::Event& event);
    void handlePausedInput(const sf::Event& event);
    void handleGameOverInput(const sf::Event& event);

    void updatePlaying(float dt);
    void renderMenu();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();

    void restartGame();

    sf::RenderWindow window;
    Map map;
    Player player;
    sf::Clock clock;
    sf::View view;
    GameState gameState{GameState::Menu};

    int selectedMenuItem{0};
};