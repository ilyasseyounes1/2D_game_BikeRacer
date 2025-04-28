#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <memory>

enum GameState { MENU, PLAYING };

class Obstacle {
protected:
    sf::Sprite sprite;
    float speed;
    float scale = 1.0f;
    
public:
    virtual ~Obstacle() = default;
    virtual void update(float deltaTime) {
        sprite.move(0, speed * deltaTime * 60.0f);
    }
    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
    float getY() const {
        return sprite.getPosition().y;
    }
    void setScale(float s) { scale = s; sprite.setScale(scale, scale); }
};

class CarObstacle : public Obstacle {
public:
    CarObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.7f); // Adjusted scale for better proportion
        speed = 3.0f;
    }
};

class ConstructionObstacle : public Obstacle {
public:
    ConstructionObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.6f); // Adjusted scale
        speed = 3.0f;
    }
};

class RoadBarrierObstacle : public Obstacle {
public:
    RoadBarrierObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.5f); // Adjusted scale
        speed = 3.0f;
    }
};

class Game {
private:
    sf::RenderWindow window;
    GameState currentState = MENU;
    
    // Bike animation variables
    sf::Texture bikeSheet;
    std::vector<sf::IntRect> frameRects;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float animationTimer = 0.0f;
    
    // Bike sprite
    sf::Sprite bike;
    const float bikeWidth = 40.0f;  // Increased for better visibility
    const float bikeHeight = 70.0f; // Increased for better visibility
    float bikeY = 450.0f;          // Moved up slightly
    float bikeSpeed = 4.0f;         // Slightly faster
    bool isBoosting = false;
    float boostTimer = 0.0f;
    float bikeRotation = 0.0f;
    const float maxRotation = 15.0f;

    // Road scrolling
    sf::Texture roadTexture;
    std::vector<sf::Sprite> roadSegments;
    float roadSpeed = 3.0f;
    const float roadSegmentHeight = 600.0f;

    // Obstacles
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    sf::Texture carTexture;
    sf::Texture constructionTexture;
    sf::Texture barrierTexture;
    float nextObstacleY = -100.0f;
    const float obstacleSpacing = 250.0f; // Increased spacing

    // Game progression
    int score = 0;
    const int difficultyInterval = 10;
    float maxTrackSpeed = 6.0f; // Slightly faster max speed

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

// In your loadBikeAnimation() function:
void loadBikeAnimation() {
    if (!bikeSheet.loadFromFile("assets/bike_sheet.png")) {
        // Create emergency fallback (red bike)
        sf::Image placeholder;
        placeholder.create(160, 70, sf::Color::Transparent);
        
        // Draw simple bike frames
        for (int i = 0; i < 4; i++) {
            int frameX = i * 40;
            // Bike body (red rectangle)
            for (int y = 20; y < 50; y++) 
                for (int x = frameX + 10; x < frameX + 30; x++) 
                    placeholder.setPixel(x, y, sf::Color::Red);
            // Wheels (black circles)
            for (int y = 0; y < 70; y++) {
                for (int x = 0; x < 40; x++) {
                    if (sqrt(pow(x-(frameX+25),2) + pow(y-60,2)) < 8 || 
                        sqrt(pow(x-(frameX+15),2) + pow(y-60,2)) < 8) {
                        placeholder.setPixel(x, y, sf::Color::Black);
                    }
                }
            }
        }
        bikeSheet.loadFromImage(placeholder);
    }
    
    bikeSheet.setSmooth(false); // Critical for pixel art
    
    // Define animation frames (40x70 each)
    frameRects.clear();
    for (int i = 0; i < 4; i++) {
        frameRects.push_back(sf::IntRect(i * 40, 0, 40, 70));
    }
    
    bike.setTexture(bikeSheet);
    bike.setTextureRect(frameRects[0]);
    bike.setOrigin(20, 35); // Center origin (40x70/2)
}

    void initRoad() {
        if (!roadTexture.loadFromFile("assets/road_texture.png")) {
            std::cerr << "Error loading road texture - creating placeholder\n";
            // Create a simple road pattern
            sf::Image roadImg;
            roadImg.create(800, 600, sf::Color(100, 100, 100)); // Gray road
            
            // Add road markings
            for (int x = 0; x < 800; x += 50) {
                for (int y = 0; y < 600; y++) {
                    if (y % 20 < 10 && x % 100 < 50) {
                        roadImg.setPixel(x, y, sf::Color::Yellow); // Yellow dashed lines
                    }
                }
            }
            roadTexture.loadFromImage(roadImg);
        }
        roadTexture.setRepeated(true);
        
        for (int i = 0; i < 2; ++i) {
            sf::Sprite segment;
            segment.setTexture(roadTexture);
            segment.setTextureRect(sf::IntRect(0, 0, 800, roadSegmentHeight));
            segment.setPosition(0, i * roadSegmentHeight - roadSegmentHeight);
            roadSegments.push_back(segment);
        }
    }

    void resetGame() {
        bike.setPosition(400, 450); // Adjusted Y position
        bikeY = 450.0f;
        bikeRotation = 0.0f;
        obstacles.clear();
        nextObstacleY = -100.0f;
        score = 0;
        roadSpeed = 2.5f; // Slightly faster starting speed
        generateInitialObstacles();
        currentFrame = 0;
        animationTimer = 0.0f;
    }

    void generateInitialObstacles() {
        for (int i = 0; i < 5; ++i) {
            addRandomObstacle();
        }
    }

    void addRandomObstacle() {
        // Adjusted obstacle positioning with better margins
        float minX = 100.0f;
        float maxX = 700.0f;
        float obstacleX = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(maxX - minX)));
        
        int obstacleType = rand() % 3;
        switch (obstacleType) {
            case 0:
                obstacles.push_back(std::make_unique<CarObstacle>(obstacleX, nextObstacleY, carTexture));
                break;
            case 1:
                obstacles.push_back(std::make_unique<ConstructionObstacle>(obstacleX, nextObstacleY, constructionTexture));
                break;
            case 2:
                obstacles.push_back(std::make_unique<RoadBarrierObstacle>(obstacleX, nextObstacleY, barrierTexture));
                break;
        }
        
        nextObstacleY -= obstacleSpacing;
    }

    void increaseDifficulty() {
        roadSpeed = std::min(maxTrackSpeed, roadSpeed * 1.05f);
    }

public:
    Game() : window(sf::VideoMode(800, 600), "Bike Race") {
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        
        std::srand(std::time(nullptr));
        
        // Load textures with better error handling
        if (!backgroundTexture.loadFromFile("assets/background.png")) {
            std::cerr << "Error loading background - using fallback\n";
            backgroundTexture.create(800, 600);
            sf::Image bgImg;
            bgImg.create(800, 600, sf::Color(135, 206, 235)); // Sky blue
            backgroundTexture.loadFromImage(bgImg);
        }
        
        if (!menuBgTexture.loadFromFile("assets/menu_bg.png")) {
            std::cerr << "Error loading menu background - using fallback\n";
            menuBgTexture.create(800, 600);
            sf::Image menuImg;
            menuImg.create(800, 600, sf::Color(50, 50, 100)); // Dark blue
            menuBgTexture.loadFromImage(menuImg);
        }
        
        if (!boostTexture.loadFromFile("assets/boost_icon.png")) {
            std::cerr << "Error loading boost texture - using fallback\n";
            boostTexture.create(30, 30);
            sf::Image boostImg;
            boostImg.create(30, 30, sf::Color::Transparent);
            // Draw a simple lightning bolt
            for (int i = 0; i < 30; i++) {
                boostImg.setPixel(15 + i/3, i, sf::Color::Yellow);
                boostImg.setPixel(15 - i/3, i, sf::Color::Yellow);
            }
            boostTexture.loadFromImage(boostImg);
        }
        
        // Load obstacle textures with fallbacks
        if (!carTexture.loadFromFile("assets/car.jpg")) {
            std::cerr << "Error loading car texture - using placeholder\n";
            carTexture.create(100, 60);
            sf::Image carImg;
            carImg.create(100, 60, sf::Color::Transparent);
            // Simple car shape
            for (int y = 20; y < 40; y++) {
                for (int x = 10; x < 90; x++) {
                    carImg.setPixel(x, y, sf::Color::Blue);
                }
            }
            // Windows
            for (int y = 25; y < 35; y++) {
                for (int x = 20; x < 40; x++) carImg.setPixel(x, y, sf::Color::Cyan);
                for (int x = 60; x < 80; x++) carImg.setPixel(x, y, sf::Color::Cyan);
            }
            carTexture.loadFromImage(carImg);
        }
        
        if (!constructionTexture.loadFromFile("assets/construction.png")) {
            std::cerr << "Error loading construction texture - using placeholder\n";
            constructionTexture.create(80, 80);
            sf::Image conImg;
            conImg.create(80, 80, sf::Color::Transparent);
            // Construction cone
            for (int y = 0; y < 80; y++) {
                int width = 40 - y/2;
                for (int x = 40 - width; x < 40 + width; x++) {
                    if (x >= 0 && x < 80) {
                        conImg.setPixel(x, y, sf::Color(255, 165, 0)); // Orange
                    }
                }
            }
            constructionTexture.loadFromImage(conImg);
        }
        
        if (!barrierTexture.loadFromFile("assets/car2.png")) {
            std::cerr << "Error loading car2 texture - using placeholder\n";
            barrierTexture.create(100, 60);
            sf::Image carImg;
            carImg.create(100, 60, sf::Color::Transparent);
            // Simple car shape
            for (int y = 20; y < 40; y++) {
                for (int x = 10; x < 90; x++) {
                    carImg.setPixel(x, y, sf::Color::Blue);
                }
            }
            // Windows
            for (int y = 25; y < 35; y++) {
                for (int x = 20; x < 40; x++) carImg.setPixel(x, y, sf::Color::Cyan);
                for (int x = 60; x < 80; x++) carImg.setPixel(x, y, sf::Color::Cyan);
            }
            carTexture.loadFromImage(carImg);
        }

        // Setup sprites
        background.setTexture(backgroundTexture);
        menuBackground.setTexture(menuBgTexture);
        boostIcon.setTexture(boostTexture);
        boostIcon.setPosition(30, 100); // Adjusted position
        boostIcon.setScale(1.5f, 1.5f); // Larger boost icon
        
        // Load bike animation and road
        loadBikeAnimation();
        initRoad();
        resetGame();

        // Menu setup
        if (!font.loadFromFile("assets/arial.ttf")) {
            std::cerr << "Error loading font - using default\n";
            font = sf::Font();
            // SFML has a default font if you don't load one
        }
        
        playText.setString("Play");
        playText.setFont(font);
        playText.setCharacterSize(40); // Larger text
        playText.setPosition(350, 250);
        playText.setFillColor(sf::Color::White);
        playText.setStyle(sf::Text::Bold);

        quitText.setString("Quit");
        quitText.setFont(font);
        quitText.setCharacterSize(40); // Larger text
        quitText.setPosition(350, 350);
        quitText.setFillColor(sf::Color::White);
        quitText.setStyle(sf::Text::Bold);

        scoreText.setFont(font);
        scoreText.setCharacterSize(28); // Larger score
        scoreText.setPosition(20, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setStyle(sf::Text::Bold);

        difficultyText.setFont(font);
        difficultyText.setCharacterSize(24); // Slightly larger
        difficultyText.setPosition(20, 60); // More spacing from score
        difficultyText.setFillColor(sf::Color::White);

        boostText.setFont(font);
        boostText.setCharacterSize(24); // Larger
        boostText.setPosition(80, 105); // Aligned with boost icon
        boostText.setFillColor(sf::Color::White);
        boostText.setString("READY");
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
            bool isTurning = false;
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                bikeX = std::max(bikeX - bikeSpeed, bikeWidth/2.0f);
                bikeRotation = -maxRotation;
                isTurning = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                bikeX = std::min(bikeX + bikeSpeed, 800.0f - bikeWidth/2.0f);
                bikeRotation = maxRotation;
                isTurning = true;
            }
            
            // Smoother rotation reset
            if (!isTurning) {
                bikeRotation *= 0.8f; // Faster return to center
                if (std::abs(bikeRotation) < 0.5f) bikeRotation = 0.0f;
            }
            
            bike.setPosition(bikeX, bikeY);
            bike.setRotation(bikeRotation);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isBoosting) {
                isBoosting = true;
                boostTimer = 0.7f; // Longer boost
                roadSpeed *= 1.7f; // More dramatic speed increase
                boostText.setString("BOOSTING!");
                frameTime = 0.05f;
            }
        }
    }

    void update(float deltaTime) {
        if (currentState == PLAYING) {
            // Smoother animation update
            animationTimer += deltaTime;
            while (animationTimer >= frameTime) {
                animationTimer -= frameTime;
                currentFrame = (currentFrame + 1) % frameRects.size();
                bike.setTextureRect(frameRects[currentFrame]);
            }

            // Boost effect with more pronounced animation
            if (isBoosting) {
                boostTimer -= deltaTime;
                bikeY = 450 - 30 * sin(boostTimer * 10.0f); // More dramatic bounce
                
                // Flash effect during boost
                if (static_cast<int>(boostTimer * 10) % 2 == 0) {
                    bike.setColor(sf::Color(255, 255, 200)); // Slightly yellow
                } else {
                    bike.setColor(sf::Color::White);
                }

                if (boostTimer <= 0) {
                    isBoosting = false;
                    roadSpeed /= 1.7f;
                    bikeY = 450;
                    bike.setColor(sf::Color::White);
                    boostText.setString("READY");
                    frameTime = 0.1f;
                }
            }

            // Road scrolling with better segment management
            for (auto& segment : roadSegments) {
                segment.move(0, roadSpeed);
                if (segment.getPosition().y > roadSegmentHeight) {
                    segment.setPosition(0, segment.getPosition().y - 2 * roadSegmentHeight);
                }
            }

            // Obstacle updates
            for (auto& obstacle : obstacles) {
                obstacle->update(deltaTime);
            }

            // Obstacle management with better spawning
            if (!obstacles.empty() && obstacles.front()->getY() > 650) { // Slightly off-screen before removal
                obstacles.erase(obstacles.begin());
                addRandomObstacle();
                score += 1 + static_cast<int>(roadSpeed / 3.0f); // Higher score at higher speeds
                
                if (score % difficultyInterval == 0) {
                    increaseDifficulty();
                }
            }

            // Collision detection with rotated bike
            sf::FloatRect bikeBounds = bike.getGlobalBounds();
            for (auto& obstacle : obstacles) {
                if (bikeBounds.intersects(obstacle->getBounds())) {
                    currentState = MENU;
                    // Add collision effect here if desired
                }
            }
        }
    }

    void render() {
        window.clear();

        if (currentState == PLAYING) {
            // Draw road segments
            for (auto& segment : roadSegments) {
                window.draw(segment);
            }
            
            // Draw obstacles behind UI elements
            for (auto& obstacle : obstacles) {
                obstacle->draw(window);
            }
            
            // Draw bike with proper layering
            window.draw(bike);
            
            // UI Elements
            scoreText.setString("SCORE: " + std::to_string(score));
            window.draw(scoreText);
            
            difficultyText.setString("SPEED: " + std::to_string(static_cast<int>(roadSpeed * 50)) + "%");
            window.draw(difficultyText);
            
            window.draw(boostIcon);
            window.draw(boostText);
        } else {
            // Menu rendering
            window.draw(menuBackground);
            
            // Add visual effects to menu buttons
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                playText.setFillColor(sf::Color(200, 255, 200)); // Light green
            } else {
                playText.setFillColor(sf::Color::White);
            }
            
            if (quitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                quitText.setFillColor(sf::Color(255, 200, 200)); // Light red
            } else {
                quitText.setFillColor(sf::Color::White);
            }
            
            window.draw(playText);
            window.draw(quitText);
            
            // Add title text
            sf::Text title("BIKE RACER", font, 60);
            title.setPosition(220, 100);
            title.setFillColor(sf::Color::White);
            title.setStyle(sf::Text::Bold);
            window.draw(title);
        }

        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0; 
}