#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Obstacle.h"

class Game {
public:
    Game();
    void run();

private:
    enum GameState { MENU, PLAYING, PAUSED, ABOUT };

    void handleInput();
    void update(float deltaTime);
    void render();
    void loadBikeAnimation();
    void initRoad();
    void resetGame();
    void generateInitialObstacles();
    void addRandomObstacle();
    void increaseDifficulty();
    void setupMenu();

    sf::RenderWindow window;
    GameState currentState = MENU;

    // Bike
    sf::Sprite bike;
    sf::Texture bikeSheet;
    std::vector<sf::IntRect> frameRects;
    int currentFrame = 0;
    float animationTimer = 0.0f;
    float frameTime = 0.1f;
    const float bikeWidth = 48.0f;
    const float bikeHeight = 84.0f;
    sf::FloatRect bikeCollisionBox;
    float bikeRotation = 0.0f;
    const float maxRotation = 15.0f;
    float bikeSpeed = 4.0f;
    float bikeY = 440.0f;

    // Road
    sf::Texture roadTexture;
    std::vector<sf::Sprite> roadSegments;
    const int roadSegmentHeight = 600;
    float roadSpeed = 2.5f;
    const float maxTrackSpeed = 8.0f;

    // Obstacles
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    float nextObstacleY = -100.0f;
    const float obstacleSpacing = 300.0f;
    const int difficultyInterval = 10;

    // Textures
    sf::Texture backgroundTexture;
    sf::Texture menuBgTexture;
    sf::Texture carTexture;
    sf::Texture car2Texture;
    sf::Texture constructionTexture;
    sf::Texture barrierTexture;
    sf::Texture parkedBikeTexture;

    // Sprites
    sf::Sprite background;
    sf::Sprite menuBackground;

    // Game time
    sf::Clock gameClock;
    sf::Time elapsedTime;

    // UI
    sf::Font font;
    sf::Text timeText;
    sf::Text difficultyText;
    sf::Text playText;
    sf::Text aboutText;
    sf::Text quitText;
    sf::Text pauseText;
    sf::Text aboutContentText;
    sf::RectangleShape playButtonBg;
    sf::RectangleShape aboutButtonBg;
    sf::RectangleShape quitButtonBg;
    sf::RectangleShape aboutPanel;
};