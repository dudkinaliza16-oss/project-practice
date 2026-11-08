#include "MenuScreen.h"
#include "Game.h"
namespace {
    constexpr float SCREEN_CENTER_X = WIDTH_WINDOW/2;
    constexpr unsigned int TITLE_FONT_SIZE = 64;
    constexpr unsigned int BUTTON_FONT_SIZE = 48;
    constexpr float TITLE_TO_BUTTON_GAP = 120.f;
    constexpr float BUTTON_SPACING = 70.f;
}

void MenuScreen::Init(const sf::Font& font, const std::string& title, const std::vector<std::string>& buttonNames, float startY) {
    selectedIndex = 0;
    buttons.clear();

    titleText.emplace(font, title, TITLE_FONT_SIZE);
    titleText->setFillColor(sf::Color::White);
    float titleWidth = titleText->getLocalBounds().size.x;
    titleText->setPosition({SCREEN_CENTER_X - titleWidth / 2.f, startY});

    for (size_t i = 0; i < buttonNames.size(); ++i) {
        sf::Text& btn = buttons.emplace_back(font, buttonNames[i], BUTTON_FONT_SIZE);
        float btnWidth = btn.getLocalBounds().size.x;
        float btnY = startY + TITLE_TO_BUTTON_GAP + static_cast<float>(i * BUTTON_SPACING);
        btn.setPosition({SCREEN_CENTER_X - btnWidth / 2.f, btnY});
    }
}

bool MenuScreen::HandleNavigation(const sf::Event& event) {
    const sf::Event::KeyPressed* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) {
        return false;
    }
    if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W) {
        selectedIndex--;
        if (selectedIndex < 0) {
            selectedIndex = static_cast<int>(buttons.size()) - 1;
        }
    }
    else if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) {
        selectedIndex++;
        if (selectedIndex >= static_cast<int>(buttons.size())) {
            selectedIndex = 0;
        }
    }
    return (key->code == sf::Keyboard::Key::Enter);
}

void MenuScreen::Draw(sf::RenderWindow& window, sf::Color bgColor) {
    window.setView(window.getDefaultView());
    window.clear(bgColor);
    if (titleText) {
        window.draw(*titleText);
    }
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (static_cast<int>(i) == selectedIndex) {
            buttons[i].setFillColor(sf::Color::Yellow);
        } else {
            buttons[i].setFillColor(sf::Color::White);
        }
        window.draw(buttons[i]);
    }
}