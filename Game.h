#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Obstacle.h"

enum GameState { MENU, PLAYING, PAUSED, ABOUT };

class Game {
private:
    sf::RenderWindow window;
    GameState currentState = MENU;
    
    sf::Texture bikeSheet;
    std::vector<sf::IntRect> frameRects;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float animationTimer = 0.0f;
    
    sf::Sprite bike;
    const float bikeWidth = 48.0f;
    const float bikeHeight = 84.0f;
    float bikeY = 440.0f;
    float bikeSpeed = 4.0f;
    bool isBoosting = false;
    float boostTimer = 0.0f;
    float bikeRotation = 0.0f;
    const float maxRotation = 15.0f;
    bool inPuddle = false;
    float puddleSlowTimer = 0.0f;
    sf::FloatRect bikeCollisionBox;

    sf::Texture roadTexture;
    std::vector<sf::Sprite> roadSegments;
    float roadSpeed = 3.0f;
    const float roadSegmentHeight = 600.0f;

    std::vector<std::unique_ptr<Obstacle>> obstacles;
    sf::Texture carTexture;
    sf::Texture car2Texture;
    sf::Texture constructionTexture;
    sf::Texture barrierTexture;
    sf::Texture parkedBikeTexture;
    sf::Texture puddleTexture;
    float nextObstacleY = -100.0f;
    const float obstacleSpacing = 170.0f;

    int score = 0;
    const int difficultyInterval = 10;
    float maxTrackSpeed = 6.0f;

    sf::Texture backgroundTexture;
    sf::Texture menuBgTexture;
    sf::Texture boostTexture;

    sf::Sprite background;
    sf::Sprite menuBackground;
    sf::Sprite boostIcon;

    sf::Font font;
    sf::Text playText;
    sf::Text quitText;
    sf::Text aboutText;
    sf::Text scoreText;
    sf::Text difficultyText;
    sf::Text boostText;
    sf::Text pauseText;
    sf::Text aboutContentText;
    sf::RectangleShape playButtonBg;
    sf::RectangleShape quitButtonBg;
    sf::RectangleShape aboutButtonBg;
    sf::RectangleShape aboutPanel;

    void loadBikeAnimation();
    void initRoad();
    void resetGame();
    void generateInitialObstacles();
    void addRandomObstacle();
    void increaseDifficulty();
    void setupMenu();

public:
    Game();
    void run();

private:
    void handleInput();
    void update(float deltaTime);
    void render();
};