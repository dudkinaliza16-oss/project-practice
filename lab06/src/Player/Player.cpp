#include "Player.h"
#include <iostream>
#include <algorithm>

namespace {
    const float PLAYER_WIDTH = 48.0f;
    const float PLAYER_HEIGHT = 48.0f;
    const float IMG_SCALE = TILE_SCALE;

    const float BODY_CENTER_X = 12.0f;
    const float FEET_Y = 48.0f;
    const float BODY_WIDTH = 20.0f;
    const float BODY_HEIGHT = 33.0f;

    const float HITBOX_WIDTH = BODY_WIDTH * IMG_SCALE;
    const float HITBOX_HEIGHT = BODY_HEIGHT * IMG_SCALE;

    constexpr float EPSILON = 0.001f;
    constexpr float ACCELERATION = 1800.0f;
    constexpr float MAX_SPEED = 300.0f;
    constexpr float FRICTION = 0.88f;
    constexpr float GRAVITY = 980.0f;
    constexpr float JUMP_FORCE = -500.0f;

    bool ProcessTileEffect(Player& player, TileType type) {
        if (type == TileType::Kill || type == TileType::Defeat) {
            player.hp = 0;
            player.state = PlayerState::Dead;
            return true;
        }
        if (type == TileType::Victory) {
            player.reachedVictory = true;
            return true;
        }
        return false;
    }
}

void ProcessPlayerInput(Player& player, float dt) {
    bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    bool moveUpward = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    if (moveLeft) {
        player.velocity.x -= ACCELERATION * dt;
    }
    if (moveRight) {
        player.velocity.x += ACCELERATION * dt;
    }
    static bool jumpKeyPressedLastFrame = false;
    if (moveUpward && !jumpKeyPressedLastFrame) {
        if (player.groundState == GroundState::OnGround || player.jumpCount < player.maxJumps) {
            player.velocity.y = JUMP_FORCE;
            player.groundState = GroundState::InAir;
            player.jumpCount++;
        }
    }
    jumpKeyPressedLastFrame = moveUpward;
    if (!moveLeft && !moveRight) {
        player.velocity.x *= FRICTION;
    }
    player.velocity.x = std::clamp(player.velocity.x, -MAX_SPEED, MAX_SPEED);
}

void UpdatePlayerMovementState(Player& player) {
    if (player.groundState == GroundState::OnGround) {
        if (std::abs(player.velocity.x) > 10.0f) {
            player.movementState = MovementState::Running;
        }
        else {
            player.movementState = MovementState::Idle;
        }
    } else {
        if (player.velocity.y < 0.0f) {
            player.movementState = MovementState::Jumping;
        }
        else {
            player.movementState = MovementState::Falling;
        }
    }
}

void HandleCollisionX(Player& player, float dt, const Map& map) {
    sf::FloatRect bounds = player.GetHitbox();
    std::vector<TileCollisionInfo> tilesX = MapGetNearbyTiles(map, bounds);
    for (size_t i = 0; i < tilesX.size(); ++i) {
        const TileCollisionInfo& tileInfo = tilesX[i];
        if (!bounds.findIntersection(tileInfo.rect)) {
            continue;
        }
        if (ProcessTileEffect(player, tileInfo.type)) {
            continue;
        }
        if (tileInfo.type == TileType::Solid) {
            sf::Vector2f position = player.sprite.getPosition();
            if (player.velocity.x > 0.f) {
                position.x = tileInfo.rect.position.x - HITBOX_WIDTH / 2.f - EPSILON;
            }
            else if (player.velocity.x < 0.f) {
                position.x = tileInfo.rect.position.x + tileInfo.rect.size.x + HITBOX_WIDTH / 2.0f + EPSILON;
            }
            player.sprite.setPosition(position);
            player.velocity.x = 0.0f;
            bounds = player.GetHitbox();
        }
    }
}

void HandleCollisionY(Player& player, float dt, const Map& map) {
    player.velocity.y += GRAVITY * dt;
    sf::FloatRect bounds = player.GetHitbox();
    player.groundState = GroundState::InAir;
    std::vector<TileCollisionInfo> tilesY = MapGetNearbyTiles(map, bounds);
    for (size_t i = 0; i < tilesY.size(); ++i) {
        const TileCollisionInfo& tileInfo = tilesY[i];
        if (!bounds.findIntersection(tileInfo.rect)) {
            continue;
        }
        if (ProcessTileEffect(player, tileInfo.type)) {
            continue;
        }
        if (tileInfo.type == TileType::Solid) {
            sf::Vector2f position = player.sprite.getPosition();
            if (player.velocity.y > 0.0f) {
                position.y = tileInfo.rect.position.y - EPSILON;
                player.groundState = GroundState::OnGround;
                player.jumpCount = 0;
            }
            else if (player.velocity.y < 0.0f) {
                position.y = tileInfo.rect.position.y + tileInfo.rect.size.y + HITBOX_HEIGHT + EPSILON;
            }
            player.sprite.setPosition(position);
            player.velocity.y = 0.0f;
            bounds = player.GetHitbox();
        }
    }
}

void Player::Init() {
    groundState = GroundState::InAir;
    movementState = MovementState::Idle;
    state = PlayerState::Alive;
    velocity = {0.0f, 0.0f};
    hp = 100;
    reachedVictory = false;
    jumpCount = 0;
    sf::Image heroImage;
    if (!heroImage.loadFromFile("../assets/hero.png")) {
        std::cerr << "Failed to load image!" << std::endl;
        return;
    }
    if (!texture.loadFromImage(heroImage)) {
        std::cerr << "Failed to load texture!" << std::endl;
        return;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect({0, 0}, {(int)PLAYER_WIDTH, (int)PLAYER_HEIGHT}));
    sprite.setScale({IMG_SCALE, IMG_SCALE});
    sprite.setOrigin({BODY_CENTER_X, FEET_Y});
    sprite.setPosition(pos);
}

void Player::Draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

void Player::Update(float dt, const Map& map) {
    if (state == PlayerState::Dead) {
        return;
    }
    ProcessPlayerInput(*this, dt);
    if (velocity.x > 10.0f) {
        sprite.setScale({IMG_SCALE, IMG_SCALE});
    }
    else if (velocity.x < -10.0f) {
        sprite.setScale({-IMG_SCALE, IMG_SCALE});
    }
    sprite.move({velocity.x * dt, 0.0f});
    HandleCollisionX(*this, dt, map);
    sprite.move({0.f, velocity.y * dt});
    HandleCollisionY(*this, dt, map);
    if (hp <= 0) {
        state = PlayerState::Dead;
    }
    UpdatePlayerMovementState(*this);
}

sf::FloatRect Player::GetHitbox() const {
    sf::Vector2f position = sprite.getPosition();
    return sf::FloatRect(
        {position.x - HITBOX_WIDTH / 2.0f, position.y - HITBOX_HEIGHT},
        {HITBOX_WIDTH, HITBOX_HEIGHT}
    );
}