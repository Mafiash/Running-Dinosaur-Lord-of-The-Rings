#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Tower.h"
#include "Dragon.h"
#include "Renderer.h"
#include "Ring.h"
#include "ScoreManager.h"
#include "Saruman.h"
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <regex>
#include <ranges>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Running Dino - LOTR");
    window.setFramerateLimit(60);

    Renderer renderer(window);
    renderer.setBackgroundIndex(1);

    float scrollSpeed = 4.f;
    sf::Clock speedClock, backgroundChangeClock, obstacleSpawnClock, lastTowerSpawnClock, lastDragonSpawnClock;
    sf::Clock ringSpawnClock, sarumanClock;
    sf::Clock lastSpawnClock;
    
    float nextObstacleTime = 1.0f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f;
    float nextRingTime = 15.0f + static_cast<float>(std::rand()) / RAND_MAX * 5.0f;;

    const float minGap = 1.0f;


    const float transitionDuration = 1.f;
    float transitionProgress = 0.f;
    bool transitioning = false;
    bool gameStarted = false;
    bool enteringName = true;
    bool showEndScreen = false;
    bool showHitboxes = false;

    std::unique_ptr<Saruman> saruman = nullptr;
    bool sarumanActive = false;
    float sarumanInterval = 10.0f + static_cast<float>(std::rand()) / RAND_MAX * 7.0f;
    float sarumanDuration = 6.0f;

    std::string playerName;
    sf::Text inputText, scoreText, highScoreText, nickText;
    sf::Font font;
    font.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/arial.ttf");

    inputText.setFont(font);
    inputText.setCharacterSize(28);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(830.f, 530.f);
    inputText.setString("");

    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(1100.f, 205.f);

    sf::Text invisibilityTimerText;
    invisibilityTimerText.setFont(font);
    invisibilityTimerText.setCharacterSize(40);
    invisibilityTimerText.setFillColor(sf::Color::Black);
    invisibilityTimerText.setPosition(1750.f, 12.f);

    sf::RectangleShape invisibilityBox(sf::Vector2f(160.f, 50.f));
    invisibilityBox.setFillColor(sf::Color::White);
    invisibilityBox.setOutlineColor(sf::Color::Black);
    invisibilityBox.setOutlineThickness(2.f);
    invisibilityBox.setPosition(
        invisibilityTimerText.getPosition().x - 10.f,
        invisibilityTimerText.getPosition().y - 8.f
    );

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(32);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setPosition(1100.f, 340.f);

    nickText.setFont(font);
    nickText.setCharacterSize(32);
    nickText.setFillColor(sf::Color::White);
    nickText.setPosition(1100.f, 470.f);

    sf::RectangleShape inputBox(sf::Vector2f(280.f, 60.f));
    inputBox.setPosition(792.f, 516.f);
    inputBox.setFillColor(sf::Color(255, 255, 255, 0));
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(3);

    sf::Texture startTexture;
    sf::Sprite startSprite;
    if (startTexture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/start.png")) {
        startSprite.setTexture(startTexture);
        startSprite.setScale(0.7f, 0.7f);
        startSprite.setPosition(600.f, 100.f);
    }

    sf::Texture endTexture;
    sf::Sprite endSprite;
    if (endTexture.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/end.png")) {
        endSprite.setTexture(endTexture);
        endSprite.setScale(0.7f, 0.7f);
        endSprite.setPosition(440.f, 100.f);
    }

    Player frodo;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::unique_ptr<Ring> ring = nullptr;
    ScoreManager scoreManager;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if ((showEndScreen || enteringName) && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    playerName.pop_back();
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                    char typedChar = static_cast<char>(event.text.unicode);
                    static std::regex allowed("[a-zA-Z0-9_]");
                    std::string s(1, typedChar);
                    if (std::regex_match(s, allowed) && playerName.length() < 12) {
                        playerName += typedChar;
                    }
                }
                inputText.setString(playerName);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (showEndScreen) {
                    showEndScreen = false;
                    enteringName = true;
                    playerName.clear();
                    inputText.setString("");
                }
                else if (enteringName && !playerName.empty()) {
                    enteringName = false;
                    gameStarted = true;
                    scrollSpeed = 4.f;
                    speedClock.restart();
                    backgroundChangeClock.restart();
                    obstacleSpawnClock.restart();                   
                    ringSpawnClock.restart();
                    sarumanClock.restart();
                    nextObstacleTime = 1.f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f;
                    nextRingTime = 15.0f + static_cast<float>(std::rand()) / RAND_MAX * 5.0f;;
                    obstacles.clear();
                    ring = nullptr;
                    saruman = nullptr;
                    sarumanActive = false;
                    frodo.reset();
                    renderer.setBackgroundIndex(1);
                    renderer.reset();
                    scoreManager.reset();
                }
            }
            if (event.key.code == sf::Keyboard::H) {
                showHitboxes = !showHitboxes;
            }
            if (!enteringName && gameStarted && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                frodo.jump();
            }
            if (!enteringName && gameStarted && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                frodo.startCrouch();
            }
            if (!enteringName && gameStarted && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) {
                frodo.stopCrouch();
            }
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                renderer.resize(event.size.width, event.size.height);
            }
        }

        if (gameStarted) {
            renderer.scrollBackground(scrollSpeed);
            scoreManager.update();

            if (!sarumanActive && sarumanClock.getElapsedTime().asSeconds() > sarumanInterval && lastSpawnClock.getElapsedTime().asSeconds() >= minGap) {
                saruman = std::make_unique<Saruman>(window.getSize().x + 50.f, 480.f);
                sarumanActive = true;
                sarumanClock.restart();
                lastSpawnClock.restart();
            }
            if (sarumanActive && sarumanClock.getElapsedTime().asSeconds() > sarumanDuration) {
                saruman = nullptr;
                sarumanActive = false;
                sarumanClock.restart();
            }

            if (speedClock.getElapsedTime().asSeconds() >= 3.f) {
                scrollSpeed += 1.0f;
                speedClock.restart();
            }

            if (backgroundChangeClock.getElapsedTime().asSeconds() >= 20.f && !transitioning) {
                transitioning = true;
                transitionProgress = 0.f;
                backgroundChangeClock.restart();
            }

            if (transitioning) {
                transitionProgress += 1.f / 60.f;
                renderer.fadeToNextBackground(transitionProgress / transitionDuration);
                if (transitionProgress >= transitionDuration) {
                    renderer.completeBackgroundTransition();
                    transitioning = false;
                }
            }

            if (!sarumanActive && obstacleSpawnClock.getElapsedTime().asSeconds() >= nextObstacleTime && lastSpawnClock.getElapsedTime().asSeconds() >= minGap) {
                float spawnX = static_cast<float>(window.getSize().x + 100);
                bool spawnDragon = std::rand() % 2 == 0;
                if (spawnDragon) {
                    obstacles.push_back(std::make_unique<Dragon>(spawnX));
                }
                else {
                    obstacles.push_back(std::make_unique<Tower>(spawnX));
                }
                obstacleSpawnClock.restart();
                lastSpawnClock.restart();
                nextObstacleTime = 2.f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f;
            }

            if (!sarumanActive && !ring && ringSpawnClock.getElapsedTime().asSeconds() >= nextRingTime && lastSpawnClock.getElapsedTime().asSeconds() >= minGap) {
                ring = std::make_unique<Ring>(static_cast<float>(window.getSize().x + 100));
                ringSpawnClock.restart();
                lastSpawnClock.restart();
                nextRingTime = 20.f + static_cast<float>(std::rand()) / RAND_MAX * 10.f;
            }

            for (auto& obs : obstacles)
                obs->update(scrollSpeed);
            if (ring) ring->update(scrollSpeed);
            if (saruman) saruman->update(scrollSpeed);

            auto filtered = obstacles | std::views::filter([](const std::unique_ptr<Obstacle>& obs) {
                return obs->getBounds().left + obs->getBounds().width >= 0;
                });
            std::vector<std::unique_ptr<Obstacle>> newObstacles;
            for (const auto& obs : filtered) {
                newObstacles.push_back(std::move(const_cast<std::unique_ptr<Obstacle>&>(obs)));
            }
            obstacles = std::move(newObstacles);

            if (ring && ring->getBounds().left + ring->getBounds().width < 0) {
                ring = nullptr;
            }

            for (const auto& obs : obstacles) {
                if (!frodo.isInvisible() && !frodo.isInGracePeriod() &&
                    frodo.getBounds().intersects(obs->getBounds())) {
                    gameStarted = false;
                    scoreManager.tryUpdateHighScore(playerName);
                    scoreText.setString(std::to_string(scoreManager.getScore()));
                    highScoreText.setString(std::to_string(scoreManager.getHighScore()));
                    nickText.setString(scoreManager.getHighScorePlayer());
                    showEndScreen = true;
                    break;
                }
            }

            if (saruman && !frodo.isInvisible() && !frodo.isInGracePeriod()) {
                for (const auto& fireball : saruman->getFireballs()) {
                    if (frodo.getBounds().intersects(fireball.getBounds())) {
                        gameStarted = false;
                        scoreManager.tryUpdateHighScore(playerName);
                        scoreText.setString(std::to_string(scoreManager.getScore()));
                        highScoreText.setString(std::to_string(scoreManager.getHighScore()));
                        nickText.setString(scoreManager.getHighScorePlayer());
                        showEndScreen = true;
                        break;
                    }
                }
            }

            if (ring && frodo.getBounds().intersects(ring->getBounds()) && !ring->isCollected()) {
                ring->collect();
                frodo.setInvisible(true);
                std::thread([&frodo]() {
                    std::this_thread::sleep_for(std::chrono::seconds(4));
                    frodo.setInvisible(false);
                    }).detach();
            }
        }

        frodo.update();

        window.clear();
        renderer.drawBackground(window, transitioning);
        for (const auto& obs : obstacles)
            obs->draw(window);
        if (ring) ring->draw(window);
        if (saruman) renderer.drawSaruman(window, *saruman);
        frodo.draw(window);
        if (showHitboxes) {
            renderer.drawHitboxes(window, frodo, obstacles);
        }
        scoreManager.draw(window, gameStarted);

        if (showEndScreen) {
            window.draw(endSprite);
            window.draw(scoreText);
            window.draw(highScoreText);
            window.draw(nickText);
        }
        else if (enteringName) {
            window.draw(startSprite);
            window.draw(inputBox);
            window.draw(inputText);
        }

        if (frodo.isInvisible()) {
            float remaining = frodo.getRemainingInvisibilityTime();
            invisibilityTimerText.setString("Inv: " + std::to_string(static_cast<int>(std::ceil(remaining))) + "s");
            window.draw(invisibilityBox);
            window.draw(invisibilityTimerText);
        }
        else {
            invisibilityTimerText.setString("");
        }

        window.display();
    }
    return 0;
}
