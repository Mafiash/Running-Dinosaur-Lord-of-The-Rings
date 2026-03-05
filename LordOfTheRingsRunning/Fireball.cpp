#include "Fireball.h"
#include <iostream>

Fireball::Fireball(float startX, float startY, float speed)
    : speed(speed), currentFrame(0) {

    std::vector<std::string> framePaths = {
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/fireball1.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/fireball2.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/fireball3.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/fireball4.png"
    };

    for (const auto& path : framePaths) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "[Fireball] B³¹d wczytywania tekstury: " << path << "\n";
        }
        frames.push_back(tex);
    }

    if (!frames.empty()) {
        sprite.setTexture(frames[0]);
    }

    sprite.setScale(1.5f, 1.5f);
    sprite.setPosition(startX, startY);
}

void Fireball::update(float newSpeed) {
    sprite.move(-newSpeed, 0.f);

    if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
        currentFrame = (currentFrame + 1) % frames.size();
        sprite.setTexture(frames[currentFrame]);
        animationClock.restart();
    }
}

void Fireball::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Fireball::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    bounds.left += bounds.width * 0.15f;
    bounds.width *= 0.7f;
    bounds.top += bounds.height * 0.15f;
    bounds.height *= 0.7f;

    return bounds;
}

bool Fireball::isOffScreen() const {
    return sprite.getPosition().x + sprite.getGlobalBounds().width < 0;
}

