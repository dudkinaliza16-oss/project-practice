#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"

using namespace sf;

struct Game {
    Player player;
    Clock clock;
    Map map;

    void Run();
};