#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Obstacle.h"
#include "Player.h"
#include "Ring.h"
#include "ScoreManager.h"
#include "Saruman.h"
#include <concepts> 


template<typename T>
concept Drawable = requires(sf::RenderWindow & w, T obj) {
    { w.draw(obj) } -> std::same_as<void>;
};

class Renderer {
public:
    Renderer(sf::RenderWindow& window);

    void draw(sf::RenderWindow& window,
        const Player& player,
        const std::vector<std::unique_ptr<Obstacle>>& obstacles,
        bool transitioning,
        Ring* ring = nullptr,
        ScoreManager* scoreManager = nullptr);

    void drawHitboxes(sf::RenderWindow& window, const Player& player,
        const std::vector<std::unique_ptr<Obstacle>>& obstacles);

    void resize(int width, int height);
    void scrollBackground(float speed);
    void fadeToNextBackground(float progress);
    void completeBackgroundTransition();
    void reset();
    void setBackgroundIndex(int index);
    void drawBackground(sf::RenderWindow& window, bool transitioning);
    void drawSaruman(sf::RenderWindow& window, const Saruman& saruman);

    template<Drawable T>
    void safeDraw(sf::RenderWindow& window, const T& drawable) {
        window.draw(drawable);
    }

private:
    std::vector<sf::Texture> backgroundTextures;
    std::vector<sf::Sprite> backgroundSprites;
    sf::RectangleShape fadeRect;
    int currentBackground;
    float backgroundX1, backgroundX2, backgroundWidth;
};
