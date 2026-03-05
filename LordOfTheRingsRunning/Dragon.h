#pragma once
#include "Obstacle.h"
#include <vector>

class Dragon : public Obstacle {
public:
    Dragon(float startX);
    void update(float speed) override;
    sf::FloatRect getBounds() const override;


private:
    std::vector<sf::Texture> frames;
    std::size_t currentFrame;
    sf::Clock animationClock;
    float baseY;   
    float sineOffset;   
    float timeAccumulator; 

};