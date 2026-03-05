#include "Obstacle.h"

void Obstacle::update(float speed) {
    sprite.move(-speed, 0.f);
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Obstacle::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += bounds.width * 0.05f;
    bounds.width *= 0.7f;
    return bounds;
}

