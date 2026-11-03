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

void HandleCollisionX(Player& player, float dt, const Map& map){
	FloatRect bounds = player.GetHitbox();
	std::vector<TileCollisionInfo> tilesX = MapGetNearbyTiles(map, bounds);

    for (size_t i = 0; i < tilesX.size(); ++i) {
        const TileCollisionInfo& tileInfo = tilesX[i];
        if (!bounds.findIntersection(tileInfo.rect)) {
            continue;
        }
        if (tileInfo.type == TileType::Kill) {
            player.hp = 0;
            continue;
        }
        if (tileInfo.type == TileType::Solid) {
            Vector2f position = player.sprite.getPosition();
            if (velocity.x > 0.f) {
                position.x = tileInfo.rect.position.x - HITBOX_WIDTH / 2.f - EPSILON;
            } else if (velocity.x < 0.f) {
                position.x = tileInfo.rect.position.x + tileInfo.rect.size.x + HITBOX_WIDTH / 2.f + EPSILON;
            }

            velocity.x = 0.f;
            bounds = GetHitbox();
        }
    }
}

void HandleCollisionY(Player& player, float dt, const Map& map){
	velocity.y += GRAVITY * dt;

    FloatRect bounds = GetHitbox();
    groundState = GroundState::InAir;
	std::vector<TileCollisionInfo> tilesY = MapGetNearbyTiles(map, bounds);
    for (size_t i = 0; i < tilesY.size(); ++i) {
        const TileCollisionInfo& tileInfo = tilesY[i];
        if (!bounds.findIntersection(tileInfo.rect)) {
            continue;
        }
        if (tileInfo.type == TileType::Kill) {
            hp = 0;
            continue;
        }
        if (tileInfo.type == TileType::Solid) {
            Vector2f position = sprite.getPosition();
            if (velocity.y > 0.0f) {
                position.y = tileInfo.rect.position.y - EPSILON;
                groundState = GroundState::OnGround;
            } else if (velocity.y < 0.0f) {
                position.y = tileInfo.rect.position.y + tileInfo.rect.size.y + HITBOX_HEIGHT + EPSILON;
            }
            sprite.setPosition(position);
            velocity.y = 0.0f;
            bounds = GetHitbox();
        }
    }


}

void Player::Init() {
	groundState = GroundState::InAir;
	movementState = MovementState::Idle;
	velocity = {0.f, 0.f};
	state = PlayerState::Alive;
	hp = 100;

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

	bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
	bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
	bool moveUpward = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W));

    if (moveLeft) {
        velocity.x -= ACCELERATION * dt;
    }
    if (moveRight) {
        velocity.x += ACCELERATION * dt;
    }

    if (moveUpward && (groundState == GroundState::OnGround)) {
        velocity.y = JUMP_FORCE;
        groundState = GroundState::InAir;
    }

    if (!moveLeft && !moveRight) {
        velocity.x *= FRICTION;
    }

    velocity.x = std::clamp(velocity.x, -MAX_SPEED, MAX_SPEED);
    if (velocity.x > 10.0f) {
        sprite.setScale({IMG_SCALE, IMG_SCALE});
    } else if (velocity.x < -10.0f) {
        sprite.setScale({-IMG_SCALE, IMG_SCALE});
    }
	sprite.move({velocity.x * dt, 0.f});
	sprite.setPosition(position);
    HandleCollisionX(map);
	sprite.move({0.f, velocity.y * dt});
    HandleCollisionY(map);

	if (groundState == GroundState::OnGround) {
	    if (std::abs(velocity.x) > 10.f) {
	        movementState = MovementState::Running;
	    } else {
	        movementState = MovementState::Idle;
	    }
	} else {
	    if (velocity.y < 0.f) {
	        movementState = MovementState::Jumping;
	    } else {
	        movementState = MovementState::Falling;
	    }
	}
}

FloatRect Player::GetHitbox() const {
    Vector2f position = sprite.getPosition();
    return FloatRect({position.x - HITBOX_WIDTH / 2.f, position.y - HITBOX_HEIGHT}, {HITBOX_WIDTH, HITBOX_HEIGHT});
}