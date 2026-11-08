#pragma once
#include <SFML/Graphics.hpp>

struct Hud {
    sf::Font font;
    sf::Text progressText{font};

    bool Init();
    void Update(float playerX, float levelWidthPixels);
    void Draw(sf::RenderWindow& window) const;
};