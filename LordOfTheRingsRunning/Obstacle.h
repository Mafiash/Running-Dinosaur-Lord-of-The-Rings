#pragma once

#include <SFML/Graphics.hpp>

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual void update(float speed);
    virtual void draw(sf::RenderWindow& window) const;
    virtual sf::FloatRect getBounds() const;

protected:
    sf::Texture texture;
    sf::Sprite sprite;
};
