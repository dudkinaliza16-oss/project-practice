#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"

struct Player {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos{400.f, 300.f};
    sf::Vector2f velocity{0.f, 0.f};
    bool isGrounded = false;

    Player() : sprite(texture) {};
    void Init();
    void Update(float dt, const Map& map);
    void Draw(sf::RenderWindow &window) const;
    sf::FloatRect GetHitbox() const;
};