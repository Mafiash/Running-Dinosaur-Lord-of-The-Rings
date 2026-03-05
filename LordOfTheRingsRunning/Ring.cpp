#include "Ring.h"

Ring::Ring(float startX) : collected(false) {
    texture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/ring.png");
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    sprite.setPosition(startX, 700.f);
}

void Ring::update(float speed) {
    if (!collected)
        sprite.move(-speed, 0.f);
}

void Ring::draw(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(sprite);
}

sf::FloatRect Ring::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Ring::isCollected() const {
    return collected;
}

void Ring::collect() {
    collected = true;
}
