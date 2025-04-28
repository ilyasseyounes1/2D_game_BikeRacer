#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

enum GameState { MENU, PLAYING };

class Game {
private:
    sf::RenderWindow window;
    GameState currentState = MENU;
    
    // Animation variables
    std::vector<sf::Texture> bikeFrames;
    int currentFrame = 0;
    float frameTime = 0.1f; // Time between frames (seconds)
    float animationTimer = 0.0f;
    
    // Bike sprite (now animated)
    sf::Sprite bike;
    const float bikeWidth = 30.0f;
    const float bikeHeight = 50.0f;
    float bikeY = 500.0f;
    float bikeSpeed = 3.0f;
    bool isBoosting = false;
    float boostTimer = 0.0f;

    // Walls
    sf::Texture wallTexture;
    std::vector<sf::Sprite> walls;
    const float wallThickness = 80.0f;
    const float wallHeight = 30.0f;
    float trackSpeed = 3.0f;
    float nextWallY = -wallHeight;
    const float wallSpacing = 200.0f;

    // Game progression
    int score = 0;
    const int difficultyInterval = 10;
    float maxTrackSpeed = 5.0f;

    // Textures
    sf::Texture backgroundTexture;
    sf::Texture menuBgTexture;
    sf::Texture boostTexture;

    // Sprites
    sf::Sprite background;
    sf::Sprite menuBackground;
    sf::Sprite boostIcon;

    // UI
    sf::Font font;
    sf::Text playText;
    sf::Text quitText;
    sf::Text scoreText;
    sf::Text difficultyText;
    sf::Text boostText;

    void loadBikeAnimation() {
        // Load each frame (assuming bike_frame0.png to bike_frame3.png)
        for (int i = 0; i < 4; ++i) {
            sf::Texture frame;
            if (!frame.loadFromFile("assets/bike_frame" + std::to_string(i) + ".png")) {
                std::cerr << "Failed to load bike frame " << i << "\n";
            }
            bikeFrames.push_back(frame);
        }
        bike.setTexture(bikeFrames[0]); // Set initial frame
    }

    void resetGame() {
        bike.setPosition(400 - bikeWidth/2, 500);
        bikeY = 500.0f;
        walls.clear();
        nextWallY = -wallHeight;
        score = 0;
        trackSpeed = 2.0f;
        generateInitialWalls();
    }

    void generateInitialWalls() {
        for (int i = 0; i < 5; ++i) {
            addWall();
        }
    }

    void addWall() {
        float wallX = 50 + rand() % (800 - 50 - static_cast<int>(wallThickness));
        
        sf::Sprite wall;
        wall.setTexture(wallTexture);
        wall.setPosition(wallX, nextWallY);
        walls.push_back(wall);
        
        nextWallY -= wallSpacing;
    }

    void increaseDifficulty() {
        trackSpeed = std::min(maxTrackSpeed, trackSpeed * 1.05f);
    }

public:
    Game() : window(sf::VideoMode(800, 600), "Bike Race") {
        std::srand(std::time(nullptr));
        
        // Load textures
        if (!wallTexture.loadFromFile("assets/wall.png")) {
            std::cerr << "Error loading wall texture\n";
        }
        if (!backgroundTexture.loadFromFile("assets/background.png")) {
            std::cerr << "Error loading background texture\n";
        }
        if (!menuBgTexture.loadFromFile("assets/menu_bg.png")) {
            std::cerr << "Error loading menu background\n";
        }
        if (!boostTexture.loadFromFile("assets/boost_icon.png")) {
            std::cerr << "Error loading boost texture\n";
        }

        // Setup sprites
        background.setTexture(backgroundTexture);
        menuBackground.setTexture(menuBgTexture);
        boostIcon.setTexture(boostTexture);
        boostIcon.setPosition(20, 100);
        
        // Load bike animation
        loadBikeAnimation();
        resetGame();

        // Menu setup
        if (!font.loadFromFile("assets/arial.ttf")) {
            std::cerr << "Error loading font\n";
        }
        playText.setString("Play");
        playText.setFont(font);
        playText.setCharacterSize(30);
        playText.setPosition(350, 250);
        playText.setFillColor(sf::Color::White);

        quitText.setString("Quit");
        quitText.setFont(font);
        quitText.setCharacterSize(30);
        quitText.setPosition(350, 350);
        quitText.setFillColor(sf::Color::White);

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setPosition(20, 20);
        scoreText.setFillColor(sf::Color::White);

        difficultyText.setFont(font);
        difficultyText.setCharacterSize(20);
        difficultyText.setPosition(20, 50);
        difficultyText.setFillColor(sf::Color::White);

        boostText.setFont(font);
        boostText.setCharacterSize(20);
        boostText.setPosition(50, 100);
        boostText.setFillColor(sf::Color::White);
        boostText.setString("Ready");
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            handleInput();
            update(deltaTime);
            render();
        }
    }

private:
    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (currentState == MENU && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentState = PLAYING;
                    resetGame();
                }
                else if (quitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                }
            }
        }

        if (currentState == PLAYING) {
            float bikeX = bike.getPosition().x;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                bikeX = std::max(bikeX - bikeSpeed, 0.0f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                bikeX = std::min(bikeX + bikeSpeed, 800.0f - bikeWidth);
            }
            bike.setPosition(bikeX, bikeY);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isBoosting) {
                isBoosting = true;
                boostTimer = 0.5f;
                trackSpeed *= 1.5f;
                boostText.setString("Active");
                frameTime = 0.05f; // Faster animation during boost
            }
        }
    }

    void update(float deltaTime) {
        if (currentState == PLAYING) {
            // Update bike animation
            animationTimer += deltaTime;
            if (animationTimer >= frameTime) {
                animationTimer = 0.0f;
                currentFrame = (currentFrame + 1) % bikeFrames.size();
                bike.setTexture(bikeFrames[currentFrame]);
            }

            if (isBoosting) {
                boostTimer -= deltaTime;
                bikeY = 500 - 20 * sin(boostTimer * 6.28f);

                if (boostTimer <= 0) {
                    isBoosting = false;
                    trackSpeed /= 1.5f;
                    bikeY = 500;
                    boostText.setString("Ready");
                    frameTime = 0.1f; // Reset to normal animation speed
                }
            }

            // Move walls
            for (auto& wall : walls) {
                wall.move(0, trackSpeed);
            }

            // Add/remove walls
            if (!walls.empty() && walls.front().getPosition().y > 600) {
                walls.erase(walls.begin());
                addWall();
                score++;
                
                if (score % difficultyInterval == 0) {
                    increaseDifficulty();
                }
            }

            // Collision detection
            for (auto& wall : walls) {
                if (bike.getGlobalBounds().intersects(wall.getGlobalBounds())) {
                    currentState = MENU;
                }
            }
        }
    }

    void render() {
        window.clear();

        if (currentState == PLAYING) {
            window.draw(background);
            
            for (auto& wall : walls) {
                window.draw(wall);
            }
            
            window.draw(bike);
            
            // UI
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            
            difficultyText.setString("Speed: " + std::to_string(static_cast<int>(trackSpeed * 50)) + "%");
            window.draw(difficultyText);
            
            window.draw(boostIcon);
            window.draw(boostText);
        } else {
            window.draw(menuBackground);
            window.draw(playText);
            window.draw(quitText);
        }

        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}