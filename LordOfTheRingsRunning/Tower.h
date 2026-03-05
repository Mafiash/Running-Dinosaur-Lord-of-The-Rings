#pragma once
#include "Obstacle.h"

class Tower : public Obstacle {
public:
    Tower(float startX);
    sf::FloatRect getBounds() const override;

};
