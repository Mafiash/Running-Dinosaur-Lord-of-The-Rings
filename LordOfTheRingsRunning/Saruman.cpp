#include "Saruman.h"
#include <iostream>

Saruman::Saruman(float startX, float groundY) {
    if (!texture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/saruman.png")) {
        std::cerr << "B³¹d ³adowania tekstury Sarumana!\n";
    }

    sprite.setTexture(texture);
    sprite.setPosition(1700.f, groundY + 160.f);  
    sprite.setScale(0.7f, 0.7f);
}

void Saruman::update(float speed) {

 
    if (fireballClock.getElapsedTime().asSeconds() > fireballInterval) {
        fireballs.emplace_back(sprite.getPosition().x, sprite.getPosition().y + 30.f, 6.f); 
        fireballClock.restart();
    }

    for (auto& fireball : fireballs) {
        fireball.update(speed + 2.f); 
    }

    fireballs.erase(
        std::remove_if(fireballs.begin(), fireballs.end(), [](const Fireball& f) {
            return f.isOffScreen();
            }),
        fireballs.end()
    );
}

std::vector<Fireball>& Saruman::getFireballs() {
    return fireballs;
}

const std::vector<Fireball>& Saruman::getFireballs() const {
    return fireballs;
}


sf::FloatRect Saruman::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Saruman::isOffScreen() const {
    return sprite.getPosition().x + sprite.getGlobalBounds().width < 0;
}

const sf::Sprite& Saruman::getSprite() const {
    return sprite;
}
