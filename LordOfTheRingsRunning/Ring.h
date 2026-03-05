#pragma once
#include <SFML/Graphics.hpp>

class Ring {
public:
    Ring(float startX);
    void update(float speed);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isCollected() const;
    void collect();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    bool collected;
};
