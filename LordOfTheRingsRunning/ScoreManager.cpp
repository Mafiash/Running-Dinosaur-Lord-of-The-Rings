#include "ScoreManager.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

ScoreManager::ScoreManager() : score(0), highScore(0) {
    if (!font.loadFromFile("C:/Users/Mateusz Smuda/Desktop/LordOfTheRingsRunning/x64/Debug/assets/arial.ttf")) {
        std::cerr << "Nie mo¿na za³adowaæ czcionki!\n";
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(1500.f, 10.f);

    scoreBox.setSize(sf::Vector2f(220.f, 50.f));
    scoreBox.setFillColor(sf::Color::White);
    scoreBox.setOutlineColor(sf::Color::Black);
    scoreBox.setOutlineThickness(2.f);
    scoreBox.setPosition(scoreText.getPosition().x - 10.f, scoreText.getPosition().y - 5.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(28);
    highScoreText.setFillColor(sf::Color::Black);
    highScoreText.setPosition(250.f, 200.f);

    loadScores();
}

void ScoreManager::update() {
    if (scoreClock.getElapsedTime().asSeconds() >= 0.1f) {
        score += 1;
        scoreClock.restart();
        scoreText.setString("Score: " + std::to_string(score));
    }
}

void ScoreManager::reset() {
    score = 0;
    scoreText.setString("Score: 0");
    scoreClock.restart();
}

void ScoreManager::tryUpdateHighScore(const std::string& playerName) {
    auto it = scoreMap.find(playerName);
    if (it == scoreMap.end() || score > it->second) {
        scoreMap[playerName] = score;
        saveScores();
    }


    highScore = 0;
    for (const auto& [name, s] : scoreMap) {
        if (s > highScore) {
            highScore = s;
            highScorePlayer = name;
        }
    }

    if (!highScorePlayer.empty()) {
        highScoreText.setString("High Score: " + std::to_string(highScore) + " (" + highScorePlayer + ")");
    }
    else {
        highScoreText.setString("High Score: " + std::to_string(highScore));
    }
}

void ScoreManager::draw(sf::RenderWindow& window, bool gameRunning) {
    if (gameRunning) {
        window.draw(scoreBox);
        window.draw(scoreText);
    }
}

int ScoreManager::getScore() const {
    return score;
}

int ScoreManager::getHighScore() const {
    return highScore;
}

std::string ScoreManager::getHighScorePlayer() const {
    return highScorePlayer;
}

void ScoreManager::loadScores() {
    scoreMap.clear();
    std::ifstream file(savePath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string name;
        int s;
        if (ss >> name >> s) {
            scoreMap[name] = s;
        }
    }

    highScore = 0;
    for (const auto& [name, s] : scoreMap) {
        if (s > highScore) {
            highScore = s;
            highScorePlayer = name;
        }
    }
}

void ScoreManager::saveScores() {
    std::filesystem::create_directories("score_data");

    std::vector<std::pair<std::string, int>> sortedScores(scoreMap.begin(), scoreMap.end());

    std::sort(sortedScores.begin(), sortedScores.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    std::ofstream file(savePath);
    if (!file.is_open()) {
        std::cerr << "[B£¥D] Nie mo¿na zapisaæ do pliku: " << savePath << "\n";
        return;
    }

    for (const auto& [name, s] : sortedScores) {
        file << name << " " << s << "\n";
    }
}

