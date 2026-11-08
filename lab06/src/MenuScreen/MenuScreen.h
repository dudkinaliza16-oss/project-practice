#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>

struct MenuScreen {
    int selectedIndex{0};

    std::optional<sf::Text> titleText;
    std::vector<sf::Text> buttons;

    void Init(
        const sf::Font& font,
        const std::string& title,
        const std::vector<std::string>& buttonNames,
        float startY
        );
    bool HandleNavigation(const sf::Event& event);
    void Draw(
        sf::RenderWindow& window,
        sf::Color bgColor
        );
};