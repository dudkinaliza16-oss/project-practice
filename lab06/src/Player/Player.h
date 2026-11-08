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
    sf::Vector2f pos{100.0f, 600.0f};
    sf::Vector2f velocity{0.f, 0.f};
	int hp = 100;
	bool reachedVictory = false;

    PlayerState state = PlayerState::Alive;
    MovementState movementState = MovementState::Idle;
    GroundState groundState = GroundState::InAir;
	int maxJumps = 2;
    int jumpCount = 0;

    Player() : sprite(texture) {};
    void Init();
    void Update(float dt, const Map& map);
    void Draw(sf::RenderWindow &window) const;
    sf::FloatRect GetHitbox() const;
};