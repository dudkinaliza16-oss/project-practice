#include "Hud.h"
#include <iostream>
#include <algorithm>
#include <string>

bool Hud::Init() {
    if (!font.openFromFile("../assets/LevTypeHryvnia-Regular.ttf")) {
        std::cerr << "Error: Failed to load font!" << std::endl;
        return false;
    }

    progressText.setFont(font);
    progressText.setCharacterSize(32);
    progressText.setFillColor(sf::Color::White);
    progressText.setPosition({20.f, 20.f});
    progressText.setString("Progress: 0%");

    return true;
}

void Hud::Update(float playerX, float levelWidthPixels) {
    if (levelWidthPixels <= 0.f) return;

    float percentage = (playerX / levelWidthPixels) * 100.f;
    percentage = std::clamp(percentage, 0.f, 100.f);

    progressText.setString("Progress: " + std::to_string(static_cast<int>(percentage)) + "%");
}

void Hud::Draw(sf::RenderWindow& window) const {
    window.draw(progressText);
}