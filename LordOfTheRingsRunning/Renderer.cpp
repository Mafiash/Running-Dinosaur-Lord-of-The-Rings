#include "Renderer.h"
#include "Saruman.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow& window) : currentBackground(0), backgroundX1(0.f), backgroundX2(0.f) {
    std::vector<std::string> backgroundPaths = {
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/tlo.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/rivendell.png"
    };

    backgroundTextures.resize(backgroundPaths.size());
    backgroundSprites.resize(2);

    for (size_t i = 0; i < backgroundPaths.size(); ++i) {
        if (!backgroundTextures[i].loadFromFile(backgroundPaths[i])) {
            std::cerr << "Nie mo¿na za³adowaæ t³a: " << backgroundPaths[i] << "\n";
        }
    }

    backgroundSprites[0].setTexture(backgroundTextures[currentBackground]);
    backgroundSprites[1].setTexture(backgroundTextures[currentBackground]);

    resize(window.getSize().x, window.getSize().y);

    fadeRect.setSize(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
}

void Renderer::resize(int width, int height) {
    float scaleX = static_cast<float>(width) / backgroundTextures[currentBackground].getSize().x;
    float scaleY = static_cast<float>(height) / backgroundTextures[currentBackground].getSize().y;

    backgroundSprites[0].setScale(scaleX, scaleY);
    backgroundSprites[1].setScale(scaleX, scaleY);

    backgroundWidth = backgroundSprites[0].getGlobalBounds().width;
    backgroundX1 = 0.f;
    backgroundX2 = backgroundWidth;

    backgroundSprites[0].setPosition(backgroundX1, 0.f);
    backgroundSprites[1].setPosition(backgroundX2, 0.f);

    fadeRect.setSize(sf::Vector2f(width, height));
}

void Renderer::scrollBackground(float speed) {
    backgroundX1 -= speed;
    backgroundX2 -= speed;

    if (backgroundX1 <= -backgroundWidth) backgroundX1 = backgroundX2 + backgroundWidth;
    if (backgroundX2 <= -backgroundWidth) backgroundX2 = backgroundX1 + backgroundWidth;

    backgroundSprites[0].setPosition(backgroundX1, 0.f);
    backgroundSprites[1].setPosition(backgroundX2, 0.f);
}

void Renderer::fadeToNextBackground(float progress) {
    float alpha = std::min(150.f, 150.f * progress);
    fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
}

void Renderer::completeBackgroundTransition() {
    currentBackground = (currentBackground + 1) % backgroundTextures.size();
    backgroundSprites[0].setTexture(backgroundTextures[currentBackground]);
    backgroundSprites[1].setTexture(backgroundTextures[currentBackground]);
    resize(static_cast<int>(fadeRect.getSize().x), static_cast<int>(fadeRect.getSize().y));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
}

void Renderer::draw(sf::RenderWindow& window,
    const Player& player,
    const std::vector<std::unique_ptr<Obstacle>>& obstacles,
    bool transitioning,
    Ring* ring,
    ScoreManager* scoreManager) {
    window.clear();
    safeDraw(window, backgroundSprites[0]);
    safeDraw(window, backgroundSprites[1]);

    for (const auto& obs : obstacles)
        obs->draw(window);

    player.draw(window);

    if (ring && !ring->isCollected())
        ring->draw(window);

    if (scoreManager)
        scoreManager->draw(window, true);

    if (transitioning)
        safeDraw(window, fadeRect);

    window.display();
}

void Renderer::reset() {
    backgroundX1 = 0.f;
    backgroundX2 = backgroundWidth;
    backgroundSprites[0].setPosition(backgroundX1, 0.f);
    backgroundSprites[1].setPosition(backgroundX2, 0.f);
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
}

void Renderer::setBackgroundIndex(int index) {
    currentBackground = index % backgroundTextures.size();
    backgroundSprites[0].setTexture(backgroundTextures[currentBackground]);
    backgroundSprites[1].setTexture(backgroundTextures[currentBackground]);
}

void Renderer::drawBackground(sf::RenderWindow& window, bool transitioning) {
    safeDraw(window, backgroundSprites[0]);
    safeDraw(window, backgroundSprites[1]);
    if (transitioning) {
        safeDraw(window, fadeRect);
    }
}

void Renderer::drawHitboxes(sf::RenderWindow& window, const Player& player, const std::vector<std::unique_ptr<Obstacle>>& obstacles) {
    sf::RectangleShape box;
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2.f);


    sf::FloatRect playerBounds = player.getBounds();
    box.setSize({ playerBounds.width, playerBounds.height });
    box.setPosition(playerBounds.left, playerBounds.top);
    window.draw(box);


    for (const auto& obs : obstacles) {
        sf::FloatRect obsBounds = obs->getBounds();
        box.setSize({ obsBounds.width, obsBounds.height });
        box.setPosition(obsBounds.left, obsBounds.top);
        window.draw(box);
    }
}

void Renderer::drawSaruman(sf::RenderWindow& window, const Saruman& saruman) {
    window.draw(saruman.getSprite());
    for (const auto& fireball : saruman.getFireballs()) {
        fireball.draw(window);
    }
}