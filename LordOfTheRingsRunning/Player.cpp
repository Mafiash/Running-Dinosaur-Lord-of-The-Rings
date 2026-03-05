#include "Player.h"

Player::Player() {
    texture1.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/frodo1.png");
    texture2.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/frodo2.png");
    jumpTexture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/frodo.png");
    crouchTexture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/frodo3.png");

    sprite.setTexture(texture1);
    sprite.setScale(0.7f, 0.7f);
    sprite.setPosition(220.f, groundY);

    velocityY = 0.f;
    onGround = true;
    invisible = false;
    crouching = false;
    animationClock.restart();
}

void Player::jump() {
    if (onGround && !crouching) {
        velocityY = -20.0f;
        onGround = false;
    }
}

void Player::startCrouch() {
    if (onGround) {
        crouching = true;
        sprite.setTexture(crouchTexture);
    }
}

void Player::stopCrouch() {
    crouching = false;
    sprite.setTexture(texture1);
}

bool Player::isCrouching() const {
    return crouching;
}

void Player::update() {
    velocityY += 0.5f;
    sprite.move(0, velocityY);

    if (sprite.getPosition().y >= groundY) {
        sprite.setPosition(sprite.getPosition().x, groundY);
        velocityY = 0.f;
        onGround = true;
    }

    if (!onGround) {
        sprite.setTexture(jumpTexture);
    }
    else if (crouching) {
        sprite.setTexture(crouchTexture);
    }
    else {
        if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
            sprite.setTexture(useFirstFrame ? texture2 : texture1);
            useFirstFrame = !useFirstFrame;
            animationClock.restart();
        }
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Player::reset() {
    sprite.setPosition(220.f, groundY);
    velocityY = 0.f;
    onGround = true;
    invisible = false;
    crouching = false;
    sprite.setColor(sf::Color(255, 255, 255, 255));
    sprite.setTexture(texture1);
    animationClock.restart();
}

void Player::setInvisible(bool state) {
    invisible = state;
    if (state) {
        sprite.setColor(sf::Color(255, 255, 255, 128));
        invisibilityClock.restart();
    }
    else {
        sprite.setColor(sf::Color(255, 255, 255, 255));
        startGracePeriod();
    }
}

bool Player::isInvisible() const {
    return invisible;
}

bool Player::isInGracePeriod() const {
    return graceClock.getElapsedTime().asSeconds() < 0.5f;
}

void Player::startGracePeriod() {
    graceClock.restart();
}

sf::FloatRect Player::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += bounds.width * 0.28f;
    bounds.width *= 0.6f;

    if (crouching) {
        bounds.top += bounds.height * 0.35f;  
        bounds.height *= 0.55f;           
    }
    else {
        bounds.top += bounds.height * 0.15f;
        bounds.height *= 0.7f;
    }

    return bounds;
}

float Player::getRemainingInvisibilityTime() const {
    if (!invisible) return 0.f;
    float elapsed = invisibilityClock.getElapsedTime().asSeconds();
    return std::max(0.f, invisibilityDuration - elapsed);

}

