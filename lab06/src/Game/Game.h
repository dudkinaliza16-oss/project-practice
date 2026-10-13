#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
using namespace sf;

struct Game {
    Player player;
    Clock clock;

    Game() = default;
    void Run();
};