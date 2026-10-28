#include "Player.h"
#include <iostream>
#include <algorithm>
using namespace sf;

const float PLAYER_WIDTH = 48.f;
const float PLAYER_HEIGHT = 48.f;
const float IMG_SCALE = TILE_SCALE;

const float BODY_CENTER_X = 12.f;
const float FEET_Y = 48.f;
const float BODY_WIDTH = 20.f;
const float BODY_HEIGHT = 33.f;

const float HITBOX_WIDTH = BODY_WIDTH * IMG_SCALE;
const float HITBOX_HEIGHT = BODY_HEIGHT * IMG_SCALE;

constexpr float EPSILON = 0.001f;
void Player::Init() {
    Image heroImage;
    if (!heroImage.loadFromFile("../assets/hero.png")) {
        std::cerr << "Failed to load image!" << std::endl;
        return;
    }
    if (!texture.loadFromImage(heroImage)) {
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
}

void Player::Update(float dt, const Map& map) {
    constexpr float ACCELERATION = 1800.0f;
    constexpr float MAX_SPEED = 300.0f;
    constexpr float FRICTION = 0.88f;
    constexpr float GRAVITY = 980.0f;
    constexpr float JUMP_FORCE = -500.0f;

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

    if (!isMoving) {
        velocity.x *= FRICTION;
    }

    velocity.x = std::clamp(velocity.x, -MAX_SPEED, MAX_SPEED);

    if (velocity.x > 10.0f) {
        sprite.setScale({IMG_SCALE, IMG_SCALE});
    } else if (velocity.x < -10.0f) {
        sprite.setScale({-IMG_SCALE, IMG_SCALE});
    }

    sprite.move({velocity.x * dt, 0.f});
    sf::FloatRect bounds = GetHitbox();

    for (const sf::FloatRect& tile : MapGetNearbyTiles(map, bounds)) {
        if (!bounds.findIntersection(tile)) {
            continue;
        }

        sf::Vector2f position = sprite.getPosition();
        if (velocity.x > 0.f) {
            position.x = tile.position.x - HITBOX_WIDTH / 2.f - EPSILON;
        } else if (velocity.x < 0.f) {
            position.x = tile.position.x + tile.size.x + HITBOX_WIDTH / 2.f + EPSILON;
        }
        sprite.setPosition(position);
        velocity.x = 0.f;
        bounds = GetHitbox();
    }

    velocity.y += GRAVITY * dt;
    sprite.move({0.f, velocity.y * dt});
    bounds = GetHitbox();
    isGrounded = false;

    for (const sf::FloatRect& tile : MapGetNearbyTiles(map, bounds)) {
        if (!bounds.findIntersection(tile)) {
            continue;
        }
        sf::Vector2f position = sprite.getPosition();
        if (velocity.y > 0.f) {
            position.y = tile.position.y - EPSILON;
            isGrounded = true;
        } else if (velocity.y < 0.f) {
            position.y = tile.position.y + tile.size.y + HITBOX_HEIGHT + EPSILON;
        }
        sprite.setPosition(position);
        velocity.y = 0.f;
        bounds = GetHitbox();
    }
}
sf::FloatRect Player::GetHitbox() const {
    sf::Vector2f position = sprite.getPosition();
    return sf::FloatRect({position.x - HITBOX_WIDTH / 2.f, position.y - HITBOX_HEIGHT}, {HITBOX_WIDTH, HITBOX_HEIGHT});
}
