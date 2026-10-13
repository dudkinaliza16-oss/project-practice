#pragma once
#include <SFML/Graphics.hpp>

struct Player {
    sf::RectangleShape body;
    sf::Vector2f position{400.f, 300.f};
    float speed{300.f};

    void Init();
    void Update(float dt);
    void Draw(sf::RenderWindow &window) const;
};