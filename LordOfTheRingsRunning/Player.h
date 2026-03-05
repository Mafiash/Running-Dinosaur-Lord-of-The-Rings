#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void jump();
    void update();
    void draw(sf::RenderWindow& window) const;
    void reset();
    void setInvisible(bool state);
    void startGracePeriod();
    bool isInvisible() const;
    bool isInGracePeriod() const;
    bool isCrouching() const;
    void startCrouch();
    void stopCrouch();
    sf::FloatRect getBounds() const;
    float getRemainingInvisibilityTime() const;


private:
    sf::Clock animationClock;
    bool useFirstFrame = true;
    sf::Clock graceClock;
    sf::Texture texture1;
    sf::Texture texture2;
    sf::Texture jumpTexture;
    sf::Texture crouchTexture;
    sf::Sprite sprite;
    float velocityY;
    bool onGround;
    bool invisible;
    bool crouching;
    sf::Clock invisibilityClock;
    static constexpr float invisibilityDuration = 4.f;

    

    static constexpr float groundY = 680.f;
};
