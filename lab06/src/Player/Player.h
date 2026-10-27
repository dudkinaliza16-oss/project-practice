#pragma once
#include <SFML/Graphics.hpp>

struct Player {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos{400.f, 300.f};
    sf::Vector2f velocity{0.f, 0.f};
    bool isGrounded = false;

    Player() : sprite(texture) {};
    void Init();
    void Update(float dt);
    void Draw(sf::RenderWindow &window) const;
};