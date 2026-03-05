#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Fireball {
public:
    Fireball(float startX, float startY, float speed);
    void update(float speed); 
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;

private:
    sf::Sprite sprite;
    std::vector<sf::Texture> frames;  
    int currentFrame;
    sf::Clock animationClock;
    float speed; 
};
