#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"

enum class PlayerState {
        Alive,
        Dead
    };

enum class MovementState {
    Idle,
    Running,
    Jumping,
    Falling
};

enum class GroundState {
    OnGround,
    InAir
};

struct Player {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos{400.f, 300.f};
    sf::Vector2f velocity{0.f, 0.f};
	int hp = 100;

    PlayerState state = PlayerState::Alive;
    MovementState movementState = MovementState::Idle;
    GroundState groundState = GroundState::InAir;

    Player() : sprite(texture) {};
    void Init();
    void Update(float dt, const Map& map);
    void Draw(sf::RenderWindow &window) const;
    sf::FloatRect GetHitbox() const;
};