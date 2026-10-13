#include "Player.h"
#include <iostream>
#include <algorithm>
using namespace sf;

const float PLAYER_WIDTH = 48.f;
const float PLAYER_HEIGHT = 48.f;
const float WINDOW_WIDTH = 800.f;
const float WINDOW_HEIGHT = 600.f;
const float IMG_SCALE = 2.5f;

void Player::Init() {
    Image heroimage;
    if (!heroimage.loadFromFile("../assets/hero.png")) {
        std::cerr << "Failed to load image!" << std::endl;
        return;
    }
    if (!texture.loadFromImage(heroimage)){
        std::cerr << "Failed to load texture!" << std::endl;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect({0, 0}, {48, 48}));

    sprite.setScale({IMG_SCALE, IMG_SCALE});

    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});

    float finalWidth = PLAYER_WIDTH * IMG_SCALE;
    float finalHeight = PLAYER_HEIGHT * IMG_SCALE;

    body.setSize(sf::Vector2f(finalWidth, finalHeight));
    body.setFillColor(sf::Color(0, 200, 255, 128));
    body.setOrigin(sf::Vector2f(finalWidth / 2.f, finalHeight / 2.f));

    sf::Vector2f initialPos{400.f, 300.f};
    body.setPosition(initialPos);
    sprite.setPosition(initialPos);
}

void Player::Draw(sf::RenderWindow &window) const {
    //window.draw(body);
    window.draw(sprite);
}

void ClampingPosition(sf::Vector2f &playerPos, float finalWidth, float finalHeight) {
    const float halfW = finalWidth / 2.f;
    const float halfH = finalHeight / 2.f;

    const float minX = halfW;
    const float maxX = WINDOW_WIDTH - halfW;
    const float minY = halfH;
    const float maxY = WINDOW_HEIGHT - halfH;

    if (playerPos.x < minX) {
        playerPos.x = minX;
    }
    if (playerPos.x > maxX) {
        playerPos.x = maxX;
    }
    if (playerPos.y < minY) {
        playerPos.y = minY;
    }
    if (playerPos.y > maxY) {
        playerPos.y = maxY;
    }
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
//        velocity.y += MAX_SPEED * dt;
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
    ClampingPosition(currentPos, body.getSize().x, body.getSize().y);

    sprite.setPosition(currentPos);
    body.setPosition(currentPos);
constexpr float FLOOR_Y = 500.0f;
float halfHeight = sprite.getLocalBounds().size.y / 2.0f;

if (sprite.getPosition().y + halfHeight >= FLOOR_Y) {
    sprite.setPosition({sprite.getPosition().x, FLOOR_Y - halfHeight});
    velocity.y = 0.0f;
    isGrounded = true;
} else {
    isGrounded = false;
}

}