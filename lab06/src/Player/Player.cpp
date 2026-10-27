#include "Player.h"
#include "Map.h"
#include <iostream>
#include <algorithm>
using namespace sf;

const float PLAYER_WIDTH = 48.f;
const float PLAYER_HEIGHT = 48.f;

const float IMG_SCALE = 3.f;
const float WORLD_WIDTH  = 30 * WORLD_TILE;
const float WORLD_HEIGHT = 20 * WORLD_TILE;
const float FLOOR_Y = WORLD_HEIGHT - WORLD_TILE;

const float BODY_CENTER_X = 12.f;
const float FEET_Y = 48.f;
const float BODY_WIDTH = 20.f;
const float BODY_HEIGHT = 32.f;

const float HITBOX_WIDTH = BODY_WIDTH * IMG_SCALE;
const float HITBOX_HEIGHT = BODY_HEIGHT * IMG_SCALE;

void Player::Init() {
    Image heroImage;
    if (!heroImage.loadFromFile("../assets/hero.png")) {
        std::cerr << "Failed to load image!" << std::endl;
        return;
    }
    if (!texture.loadFromImage(heroImage)){
        std::cerr << "Failed to load texture!" << std::endl;
        return;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect({0, 0}, {(int)PLAYER_WIDTH, (int)PLAYER_HEIGHT}));
    sprite.setScale({IMG_SCALE, IMG_SCALE});
    sprite.setOrigin({BODY_CENTER_X, FEET_Y});
    sprite.setPosition(pos);
}

void Player::Draw(sf::RenderWindow &window) const {
    window.draw(sprite);

	sf::Vector2f p = sprite.getPosition();
    sf::RectangleShape box({HITBOX_WIDTH, HITBOX_HEIGHT});
    box.setPosition({p.x - HITBOX_WIDTH / 2.f, p.y - HITBOX_HEIGHT});
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(1.f);
    window.draw(box);
}

void ClampingPosition(sf::Vector2f& feetPos) {
    feetPos.x = std::clamp(
        feetPos.x,
        HITBOX_WIDTH / 2.f,
        WORLD_WIDTH - HITBOX_WIDTH / 2.f
    );

    feetPos.y = std::clamp(
        feetPos.y,
        HITBOX_HEIGHT,
        WORLD_HEIGHT
    );
}

void Player::Update(float dt) {
	constexpr float ACCELERATION = 1800.0f;
	constexpr float MAX_SPEED = 300.0f;
	constexpr float FRICTION = 0.88f;
	constexpr float GRAVITY = 980.0f;
	constexpr float JUMP_FORCE = -450.0f;

    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        velocity.x -= ACCELERATION * dt;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity.x += ACCELERATION * dt;
        isMoving = true;
    }
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
    	sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
    	sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) && isGrounded) {
	    velocity.y = JUMP_FORCE;
    	isGrounded = false;
	}
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)
//        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
//    }

    if (!isMoving) {
        velocity.x *= FRICTION;
    }
	if (!isGrounded) {
	    velocity.y += GRAVITY * dt;
	}

	velocity.x = std::clamp(velocity.x, -MAX_SPEED, MAX_SPEED);

	if (velocity.x > 10.0f) {
    	sprite.setScale({IMG_SCALE, IMG_SCALE});
	} else if (velocity.x < -10.0f) {
    	sprite.setScale({-IMG_SCALE, IMG_SCALE});
	}
    sprite.move(velocity * dt);
    sf::Vector2f currentPos = sprite.getPosition();

	ClampingPosition(currentPos);
    if (currentPos.y >= FLOOR_Y) {
        currentPos.y = FLOOR_Y;
        velocity.y = 0.0f;
        isGrounded = true;
    } else {
        isGrounded = false;
    }

    sprite.setPosition(currentPos);

}