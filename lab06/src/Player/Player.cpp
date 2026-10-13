#include "Player.h"

const float PLAYER_WIDTH = 40.f;
const float PLAYER_HEIGHT = 50.f;
const float WINDOW_WIDTH = 800.f;
const float WINDOW_HEIGHT = 600.f;

void Player::Init() {
    body.setSize(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT));
    body.setFillColor(sf::Color(0, 200, 255));
    body.setOrigin(sf::Vector2f(PLAYER_WIDTH / 2.f, PLAYER_HEIGHT / 2.f));
    body.setPosition(position);
}

void Player::Draw(sf::RenderWindow &window) const {
    window.draw(body);
}

void ClampingPosition(sf::Vector2f &playerPos) {
    const float minX = PLAYER_WIDTH / 2.f;
    const float maxX = WINDOW_WIDTH - (PLAYER_WIDTH / 2.f);
    const float minY = PLAYER_HEIGHT / 2.f;
    const float maxY = WINDOW_HEIGHT - (PLAYER_HEIGHT / 2.f);

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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        position.x -= speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        position.x += speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        position.y -= speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        position.y += speed * dt;
    }

    ClampingPosition(position);
    body.setPosition(position);
}