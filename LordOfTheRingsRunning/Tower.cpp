#include "Tower.h"

Tower::Tower(float startX) {
    texture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/wieza.png");
    sprite.setTexture(texture);
    sprite.setScale(0.6f, 0.6f);
    sprite.setPosition(startX, 650.f);
}

sf::FloatRect Tower::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    bounds.top += bounds.height * 0.09f;
    bounds.height *= 0.85f;
    bounds.left += bounds.width * 0.18f;
    bounds.width *= 0.6f;
    return bounds;
}



