#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class ScoreManager {
public:
    ScoreManager();
    void update();
    void draw(sf::RenderWindow& window, bool gameRunning);
    void reset();
    void tryUpdateHighScore(const std::string& playerName);
    std::string getHighScorePlayer() const;
    int getScore() const;
    int getHighScore() const;

private:
    void loadScores();
    void saveScores(); 

    int score;
    int highScore;
    std::string highScorePlayer;

    std::unordered_map<std::string, int> scoreMap; 

    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Clock scoreClock;
    sf::RectangleShape scoreBox;

    const std::string savePath = "score_data/scores.txt"; 
};
