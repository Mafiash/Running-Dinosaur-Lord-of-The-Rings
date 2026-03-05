#include "Dragon.h"
#include <iostream>
#include <cmath> 

Dragon::Dragon(float startX) : currentFrame(0), timeAccumulator(0.f) {
    
    std::vector<std::string> paths = {
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/dragon1.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/dragon2.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/dragon3.png",
        "C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/dragon2.png"
    };

    for (const auto& path : paths) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "B³¹d wczytywania: " << path << "\n";
        }
        frames.push_back(tex);
    }

    if (!frames.empty()) {
        sprite.setTexture(frames[0]);
    }

   
    baseY = 470.f + static_cast<float>(std::rand() % 61 - 30);
    sineOffset = static_cast<float>(std::rand() % 100); 

    sprite.setPosition(startX, baseY);
    sprite.setScale(1.5f, 1.5f);
}

void Dragon::update(float speed) {
    sprite.move(-speed, 0.f);

  
    if (animationClock.getElapsedTime().asSeconds() > 0.12f) {
        currentFrame = (currentFrame + 1) % frames.size();
        sprite.setTexture(frames[currentFrame]);
        animationClock.restart();
    }

   
    timeAccumulator += 0.02f;
    float wave = std::sin(timeAccumulator + sineOffset) * 55.f; 

    sf::Vector2f pos = sprite.getPosition();
    sprite.setPosition(pos.x, baseY + wave);
}

sf::FloatRect Dragon::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    bounds.left += bounds.width * 0.17f;
    bounds.width *= 0.78f;

    bounds.top += bounds.height * 0.15f;
    bounds.height *= 0.60f;

    return bounds;
}
