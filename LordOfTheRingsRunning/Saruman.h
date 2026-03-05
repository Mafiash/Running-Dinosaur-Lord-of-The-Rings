#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Fireball.h"

class Saruman {
public:
    Saruman(float startX, float groundY);

    void update(float speed);
    std::vector<Fireball>& getFireballs();
    const std::vector<Fireball>& getFireballs() const;
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;
    const sf::Sprite& getSprite() const;


private:
    sf::Sprite sprite;
    sf::Texture texture;

    std::vector<Fireball> fireballs;
    sf::Clock fireballClock;
    float fireballInterval = 3.f;
};
