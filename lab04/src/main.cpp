#include <optional>
#include <SFML/Graphics.hpp>

const int WIDTH_WINDOW = 800;
const int HEIGHT_WINDOW = 600;

namespace ShipConfig {
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

struct Ship {
    sf::ConvexShape hull;
    sf::RectangleShape mast;
    sf::ConvexShape sail;
    sf::CircleShape printOfSail;
};

void initShip(Ship &ship, sf::Vector2f basePosition) {
    ship.hull.setPointCount(4);
    ship.hull.setPoint(0, {0.f, 0.f});
    ship.hull.setPoint(1, {ShipConfig::HULL_TOP_WIDTH, 0.f});
    ship.hull.setPoint(2, {ShipConfig::HULL_TOP_WIDTH - ShipConfig::HULL_SIDE_OFFSET, ShipConfig::HULL_HEIGHT});
    ship.hull.setPoint(3, {ShipConfig::HULL_SIDE_OFFSET, ShipConfig::HULL_HEIGHT});

    ship.hull.setFillColor(ShipConfig::HULL_COLOR);
    ship.hull.setOutlineThickness(ShipConfig::OUTLINE_THICKNESS);
    ship.hull.setOutlineColor(ShipConfig::OUTLINE_COLOR);
    ship.hull.setOrigin({ShipConfig::HULL_TOP_WIDTH / 2.f, 0.f});
    ship.hull.setPosition(basePosition);

    ship.mast.setSize({ShipConfig::MAST_WIDTH, ShipConfig::MAST_HEIGHT});
    ship.mast.setFillColor(ShipConfig::MAST_COLOR);
    ship.mast.setOutlineThickness(ShipConfig::OUTLINE_THICKNESS);
    ship.mast.setOutlineColor(ShipConfig::OUTLINE_COLOR);
    ship.mast.setOrigin({ShipConfig::MAST_WIDTH / 2.f, ShipConfig::MAST_HEIGHT});
    ship.mast.setPosition(basePosition);

    ship.sail.setPointCount(3);
    ship.sail.setPoint(0, {0.f, 0.f});
    ship.sail.setPoint(1, {ShipConfig::SAIL_WIDTH, ShipConfig::SAIL_HEIGHT});
    ship.sail.setPoint(2, {0.f, ShipConfig::SAIL_HEIGHT});

    ship.sail.setFillColor(ShipConfig::SAIL_COLOR);
    ship.sail.setOutlineThickness(ShipConfig::OUTLINE_THICKNESS);
    ship.sail.setOutlineColor(ShipConfig::OUTLINE_COLOR);

    sf::Vector2f sailPos = basePosition + sf::Vector2f(ShipConfig::MAST_WIDTH / 2.f + ShipConfig::OUTLINE_THICKNESS,
        -ShipConfig::MAST_HEIGHT + 15.f
                           );
    ship.sail.setOrigin({0.f, 0.f});
    ship.sail.setPosition(sailPos);

    ship.printOfSail.setRadius(ShipConfig::PRINT_RADIUS);
    ship.printOfSail.setFillColor(ShipConfig::PRINT_COLOR);
    ship.printOfSail.setOutlineThickness(ShipConfig::OUTLINE_THICKNESS);
    ship.printOfSail.setOutlineColor(ShipConfig::OUTLINE_COLOR);
    ship.printOfSail.setOrigin({ShipConfig::PRINT_RADIUS, ShipConfig::PRINT_RADIUS});

    sf::Vector2f printPos = sailPos + sf::Vector2f(ShipConfig::SAIL_WIDTH / 3.f, ShipConfig::SAIL_HEIGHT * (2.f / 3.f));
    ship.printOfSail.setPosition(printPos);
}

void drawShip(sf::RenderWindow &window, const Ship &ship) {
    window.draw(ship.mast);
    window.draw(ship.hull);
    window.draw(ship.sail);
    window.draw(ship.printOfSail);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({WIDTH_WINDOW, HEIGHT_WINDOW}), "Dudkina EM");
    window.setFramerateLimit(60);

    Ship ship;
    initShip(ship, {WIDTH_WINDOW / 2.f, HEIGHT_WINDOW * 0.75f});

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(62, 95, 138));
        drawShip(window, ship);
        window.display();
    }

    return 0;
}
