#pragma once
#include <SFML/Graphics.hpp>

struct Player {
    sf::Sprite sprite;
    sf::Texture texture;
    sf::RectangleShape body;
    sf::Vector2f velocity{0.f, 0.f};
    bool isGrounded;

    Player() : sprite(texture) {};
    void Init();
    void Update(float dt);
    void Draw(sf::RenderWindow &window) const;
};