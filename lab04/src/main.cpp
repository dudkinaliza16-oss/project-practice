#include <optional>
#include <iostream>
#include <SFML/Graphics.hpp>

const int WIDTH_WINDOW = 800;
const int HEIGHT_WINDOW = 600;

namespace PlayerConfig {
    const float HULL_TOP_WIDTH = 280.f;
    const float HULL_BOTTOM_WIDTH = 160.f;
    const float HULL_HEIGHT = 80.f;
    const float HULL_SIDE_OFFSET = (HULL_TOP_WIDTH - HULL_BOTTOM_WIDTH) / 2.f;

    const float MAST_WIDTH = 10.f;
    const float MAST_HEIGHT = 180.f;

    const float PRINT_RADIUS = 18.f;

    const float OUTLINE_THICKNESS = 2.f;

    const float SAIL_WIDTH = 90.f;
    const float SAIL_HEIGHT = 120.f;

    const sf::Color HULL_COLOR(sf::Color::Green);
    const sf::Color MAST_COLOR(sf::Color::Yellow);
    const sf::Color SAIL_COLOR(sf::Color::White);
    const sf::Color PRINT_COLOR(sf::Color::Red);
    const sf::Color OUTLINE_COLOR(sf::Color::Black);
}

struct Player {
    sf::ConvexShape hull;
    sf::RectangleShape mast;
    sf::ConvexShape sail;
    sf::CircleShape printOfSail;
    const float speed = 300.f;
    sf::Vector2f position = {WIDTH_WINDOW / 2.f, HEIGHT_WINDOW * 0.75f};
};

// struct Enemy {
//     sf::CircleShape body;
//     sf::Vector2f velocity = {200.0f, 150.0f};
//     sf::Vector2f position = {200.0f, 200.0f};
//     float radius = 20.f;
// };

void initPlayer(Player &player, sf::Vector2f basePosition) {
    player.hull.setPointCount(4);
    player.hull.setPoint(0, {0.f, 0.f});
    player.hull.setPoint(1, {PlayerConfig::HULL_TOP_WIDTH, 0.f});
    player.hull.setPoint(2, {PlayerConfig::HULL_TOP_WIDTH - PlayerConfig::HULL_SIDE_OFFSET, PlayerConfig::HULL_HEIGHT});
    player.hull.setPoint(3, {PlayerConfig::HULL_SIDE_OFFSET, PlayerConfig::HULL_HEIGHT});

    player.hull.setFillColor(PlayerConfig::HULL_COLOR);
    player.hull.setOutlineThickness(PlayerConfig::OUTLINE_THICKNESS);
    player.hull.setOutlineColor(PlayerConfig::OUTLINE_COLOR);
    player.hull.setOrigin({PlayerConfig::HULL_TOP_WIDTH / 2.f, 0.f});
    player.hull.setPosition(basePosition);

    player.mast.setSize({PlayerConfig::MAST_WIDTH, PlayerConfig::MAST_HEIGHT});
    player.mast.setFillColor(PlayerConfig::MAST_COLOR);
    player.mast.setOutlineThickness(PlayerConfig::OUTLINE_THICKNESS);
    player.mast.setOutlineColor(PlayerConfig::OUTLINE_COLOR);
    player.mast.setOrigin({PlayerConfig::MAST_WIDTH / 2.f, PlayerConfig::MAST_HEIGHT});
    player.mast.setPosition(basePosition);

    player.sail.setPointCount(3);
    player.sail.setPoint(0, {0.f, 0.f});
    player.sail.setPoint(1, {PlayerConfig::SAIL_WIDTH, PlayerConfig::SAIL_HEIGHT});
    player.sail.setPoint(2, {0.f, PlayerConfig::SAIL_HEIGHT});

    player.sail.setFillColor(PlayerConfig::SAIL_COLOR);
    player.sail.setOutlineThickness(PlayerConfig::OUTLINE_THICKNESS);
    player.sail.setOutlineColor(PlayerConfig::OUTLINE_COLOR);

    sf::Vector2f sailPos = basePosition + sf::Vector2f(PlayerConfig::MAST_WIDTH / 2.f + PlayerConfig::OUTLINE_THICKNESS,
        -PlayerConfig::MAST_HEIGHT + 15.f);
    player.sail.setOrigin({0.f, 0.f});
    player.sail.setPosition(sailPos);

    player.printOfSail.setRadius(PlayerConfig::PRINT_RADIUS);
    player.printOfSail.setFillColor(PlayerConfig::PRINT_COLOR);
    player.printOfSail.setOutlineThickness(PlayerConfig::OUTLINE_THICKNESS);
    player.printOfSail.setOutlineColor(PlayerConfig::OUTLINE_COLOR);
    player.printOfSail.setOrigin({PlayerConfig::PRINT_RADIUS, PlayerConfig::PRINT_RADIUS});

    sf::Vector2f printPos = sailPos + sf::Vector2f(PlayerConfig::SAIL_WIDTH / 3.f, PlayerConfig::SAIL_HEIGHT * (2.f / 3.f));
    player.printOfSail.setPosition(printPos);
}

// void initEnemy(Enemy& enemy) {
//     enemy.body.setRadius(enemy.radius);
//     enemy.body.setFillColor(sf::Color::Red);
//     enemy.body.setOrigin({enemy.radius, enemy.radius});
//     enemy.body.setPosition(enemy.position);
// }

void drawPlayer(sf::RenderWindow &window, const Player &player) {
    window.draw(player.mast);
    window.draw(player.hull);
    window.draw(player.sail);
    window.draw(player.printOfSail);
}

void ClampingPlayerPosition(sf::Vector2f& playerPos) {
    const float minX = PlayerConfig::HULL_TOP_WIDTH / 2.f + PlayerConfig::OUTLINE_THICKNESS;
    const float maxX = WIDTH_WINDOW - (PlayerConfig::HULL_TOP_WIDTH/2.0f + PlayerConfig::OUTLINE_THICKNESS);
    const float minY = PlayerConfig::MAST_HEIGHT + PlayerConfig::OUTLINE_THICKNESS;
    const float maxY = HEIGHT_WINDOW - (PlayerConfig::HULL_HEIGHT + PlayerConfig::OUTLINE_THICKNESS);

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

void updatePlayer(Player &player, float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        player.position.x -= player.speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player.position.x += player.speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        player.position.y -= player.speed * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        player.position.y += player.speed * dt;
    }
    ClampingPlayerPosition(player.position);

    player.hull.setPosition(player.position);
    player.mast.setPosition(player.position);

    sf::Vector2f sailPos = player.position + sf::Vector2f(PlayerConfig::MAST_WIDTH / 2.f + PlayerConfig::OUTLINE_THICKNESS, -PlayerConfig::MAST_HEIGHT + 15.f);
    player.sail.setPosition(sailPos);

    sf::Vector2f printPos = sailPos + sf::Vector2f(PlayerConfig::SAIL_WIDTH / 3.f, PlayerConfig::SAIL_HEIGHT * (2.f / 3.f));
    player.printOfSail.setPosition(printPos);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Dudkina EM");
    window.setFramerateLimit(60);

    Player ship;
    initPlayer(ship, ship.position);

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        updatePlayer(ship, dt);
        window.clear(sf::Color(62, 95, 138));
        drawPlayer(window, ship);
        window.display();
    }

    return 0;
}
