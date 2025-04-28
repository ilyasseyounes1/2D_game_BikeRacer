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
    float getPosition() const {
        return sprite.getPosition().x;
    }
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    void setScale(float s) { scale = s; sprite.setScale(scale, scale); }
};

class CarObstacle : public Obstacle {
public:
    CarObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.7f);
        speed = 3.0f;
    }
};

class ConstructionObstacle : public Obstacle {
public:
    ConstructionObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.8f); // Increased from 0.6f to 0.8f
        speed = 3.0f;
    }
};

class RoadBarrierObstacle : public Obstacle {
public:
    RoadBarrierObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.5f);
        speed = 3.0f;
    }
};

class ParkedBikeObstacle : public Obstacle {
public:
    ParkedBikeObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.6f);
        speed = 3.0f;
    }
};

class PuddleObstacle : public Obstacle {
public:
    PuddleObstacle(float x, float y, const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        setScale(0.8f);
        speed = 3.0f;
    }
};

class Game {
private:
    sf::RenderWindow window;
    GameState currentState = MENU;
    
    // Bike animation
    sf::Texture bikeSheet;
    std::vector<sf::IntRect> frameRects;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float animationTimer = 0.0f;
    
    // Bike sprite
    sf::Sprite bike;
    const float bikeWidth = 40.0f;
    const float bikeHeight = 70.0f;
    float bikeY = 450.0f;
    float bikeSpeed = 4.0f;
    bool isBoosting = false;
    float boostTimer = 0.0f;
    float bikeRotation = 0.0f;
    const float maxRotation = 15.0f;
    bool inPuddle = false;
    float puddleSlowTimer = 0.0f;

    // Road
    sf::Texture roadTexture;
    std::vector<sf::Sprite> roadSegments;
    float roadSpeed = 3.0f;
    const float roadSegmentHeight = 600.0f;

    // Obstacles
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    sf::Texture carTexture;
    sf::Texture constructionTexture;
    sf::Texture barrierTexture;
    sf::Texture parkedBikeTexture;
    sf::Texture puddleTexture;
    float nextObstacleY = -100.0f;
    const float obstacleSpacing = 250.0f;

    // Game state
    int score = 0;
    const int difficultyInterval = 10;
    float maxTrackSpeed = 6.0f;

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
        if (!bikeSheet.loadFromFile("assets/bike_sheet.png")) {
            std::cerr << "Error loading bike sprite sheet - creating placeholder\n";
            sf::Image placeholder;
            placeholder.create(160, 70, sf::Color::Transparent);
            
            // Draw 4-frame bike animation (red bike with black wheels)
            for (int i = 0; i < 4; i++) {
                int frameX = i * 40;
                // Bike frame
                for (int y = 20; y < 50; y++) {
                    for (int x = frameX + 10; x < frameX + 30; x++) {
                        placeholder.setPixel(x, y, sf::Color::Red);
                    }
                }
                // Wheels
                for (int y = 0; y < 70; y++) {
                    for (int x = 0; x < 40; x++) {
                        float dist1 = std::hypot(x-(frameX+25), y-60);
                        float dist2 = std::hypot(x-(frameX+15), y-60);
                        if (dist1 < 8 || dist2 < 8) {
                            placeholder.setPixel(x, y, sf::Color::Black);
                        }
                    }
                }
            }
            bikeSheet.loadFromImage(placeholder);
        }
        
        bikeSheet.setSmooth(false);
        frameRects.clear();
        for (int i = 0; i < 4; ++i) {
            frameRects.push_back(sf::IntRect(i * 40, 0, 40, 70));
        }
        
        bike.setTexture(bikeSheet);
        bike.setTextureRect(frameRects[0]);
        bike.setOrigin(bikeWidth/2, bikeHeight/2);
    }

    void initRoad() {
        if (!roadTexture.loadFromFile("assets/road_texture.png")) {
            std::cerr << "Error loading road texture - creating placeholder\n";
            sf::Image roadImg;
            roadImg.create(800, 600, sf::Color(100, 100, 100));
            for (int x = 0; x < 800; x += 50) {
                for (int y = 0; y < 600; y++) {
                    if (y % 20 < 10 && x % 100 < 50) {
                        roadImg.setPixel(x, y, sf::Color::Yellow);
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
        bike.setPosition(400, 450);
        bikeY = 450.0f;
        bikeRotation = 0.0f;
        obstacles.clear();
        nextObstacleY = -100.0f;
        score = 0;
        roadSpeed = 2.5f;
        bikeSpeed = 4.0f;
        inPuddle = false;
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
        float obstacleX = 100 + rand() % 600;
        
        int obstacleType = rand() % 5; // Now 5 types (0-4)
        switch (obstacleType) {
            case 0: obstacles.push_back(std::make_unique<CarObstacle>(obstacleX, nextObstacleY, carTexture)); break;
            case 1: obstacles.push_back(std::make_unique<ConstructionObstacle>(obstacleX, nextObstacleY, constructionTexture)); break;
            case 2: obstacles.push_back(std::make_unique<RoadBarrierObstacle>(obstacleX, nextObstacleY, barrierTexture)); break;
            case 3: obstacles.push_back(std::make_unique<ParkedBikeObstacle>(obstacleX, nextObstacleY, parkedBikeTexture)); break;
            case 4: obstacles.push_back(std::make_unique<PuddleObstacle>(obstacleX, nextObstacleY, puddleTexture)); break;
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
        
        // Load textures
        if (!backgroundTexture.loadFromFile("assets/background.png")) {
            sf::Image bgImg;
            bgImg.create(800, 600, sf::Color(135, 206, 235));
            backgroundTexture.loadFromImage(bgImg);
        }
        
        if (!menuBgTexture.loadFromFile("assets/menu_bg.png")) {
            sf::Image menuImg;
            menuImg.create(800, 600, sf::Color(50, 50, 100));
            menuBgTexture.loadFromImage(menuImg);
        }
        
        if (!boostTexture.loadFromFile("assets/boost_icon.png")) {
            sf::Image boostImg;
            boostImg.create(30, 30, sf::Color::Transparent);
            for (int i = 0; i < 30; i++) {
                boostImg.setPixel(15 + i/3, i, sf::Color::Yellow);
                boostImg.setPixel(15 - i/3, i, sf::Color::Yellow);
            }
            boostTexture.loadFromImage(boostImg);
        }
        
        // Obstacle textures
        if (!carTexture.loadFromFile("assets/car.jpg")) {
            sf::Image carImg;
            carImg.create(100, 60, sf::Color::Transparent);
            for (int y = 20; y < 40; y++) {
                for (int x = 10; x < 90; x++) carImg.setPixel(x, y, sf::Color::Blue);
            }
            carTexture.loadFromImage(carImg);
        }
        
        if (!constructionTexture.loadFromFile("assets/construction.png")) {
            sf::Image conImg;
            conImg.create(100, 100, sf::Color::Transparent); // Larger construction
            for (int y = 0; y < 100; y++) {
                int width = 50 - y/2;
                for (int x = 50 - width; x < 50 + width; x++) {
                    if (x >= 0 && x < 100) conImg.setPixel(x, y, sf::Color(255, 165, 0));
                }
            }
            constructionTexture.loadFromImage(conImg);
        }
        
        if (!barrierTexture.loadFromFile("assets/barrier.png")) {
            sf::Image barImg;
            barImg.create(120, 30, sf::Color::Transparent);
            for (int y = 0; y < 30; y++) {
                for (int x = 0; x < 120; x++) {
                    if ((x + y) % 20 < 10) barImg.setPixel(x, y, sf::Color::Red);
                    else barImg.setPixel(x, y, sf::Color::White);
                }
            }
            barrierTexture.loadFromImage(barImg);
        }
        
        // New obstacle textures
        if (!parkedBikeTexture.loadFromFile("assets/parked_bike.png")) {
            // Create placeholder matching 200x67 dimensions
            sf::Image img;
            img.create(200, 67, sf::Color::Transparent);
            
            // Draw detailed blue bike (proportional to 200x67)
            for (int y = 30; y < 50; y++) {
                for (int x = 50; x < 150; x++) {
                    img.setPixel(x, y, sf::Color::Blue); // Bike frame
                }
            }
            // Wheels (larger to match new scale)
            for (int y = 0; y < 67; y++) {
                for (int x = 0; x < 200; x++) {
                    if (std::hypot(x-60, y-60) < 15 || std::hypot(x-140, y-60) < 15) {
                        img.setPixel(x, y, sf::Color::Black);
                    }
                }
            }
            parkedBikeTexture.loadFromImage(img);
        }
        
        if (!puddleTexture.loadFromFile("assets/puddle.png")) {
            sf::Image puddleImg;
            puddleImg.create(100, 30, sf::Color::Transparent);
            // Draw blue oval puddle
            for (int y = 0; y < 30; y++) {
                for (int x = 0; x < 100; x++) {
                    if (std::hypot(x-50, y-15) < 30) puddleImg.setPixel(x, y, sf::Color(0, 0, 255, 150));
                }
            }
            puddleTexture.loadFromImage(puddleImg);
        }

        // Setup sprites
        background.setTexture(backgroundTexture);
        menuBackground.setTexture(menuBgTexture);
        boostIcon.setTexture(boostTexture);
        boostIcon.setPosition(30, 100);
        boostIcon.setScale(1.5f, 1.5f);
        
        // Load bike animation and road
        loadBikeAnimation();
        initRoad();
        resetGame();

        // Menu setup
        if (!font.loadFromFile("assets/arial.ttf")) {
            font = sf::Font();
        }
        
        playText.setString("PLAY");
        playText.setFont(font);
        playText.setCharacterSize(40);
        playText.setPosition(350, 250);
        playText.setFillColor(sf::Color::Black);
        playText.setStyle(sf::Text::Bold);

        quitText.setString("QUIT");
        quitText.setFont(font);
        quitText.setCharacterSize(40);
        quitText.setPosition(350, 350);
        quitText.setFillColor(sf::Color::Black);
        quitText.setStyle(sf::Text::Bold);

        scoreText.setFont(font);
        scoreText.setCharacterSize(28);
        scoreText.setPosition(20, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setStyle(sf::Text::Bold);

        difficultyText.setFont(font);
        difficultyText.setCharacterSize(24);
        difficultyText.setPosition(20, 60);
        difficultyText.setFillColor(sf::Color::White);

        boostText.setFont(font);
        boostText.setCharacterSize(24);
        boostText.setPosition(80, 105);
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
            
            if (!isTurning) {
                bikeRotation *= 0.8f;
                if (std::abs(bikeRotation) < 0.5f) bikeRotation = 0.0f;
            }
            
            bike.setPosition(bikeX, bikeY);
            bike.setRotation(bikeRotation);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isBoosting) {
                isBoosting = true;
                boostTimer = 0.7f;
                roadSpeed *= 1.7f;
                boostText.setString("BOOSTING!");
                frameTime = 0.05f;
            }
        }
    }

    void update(float deltaTime) {
        if (currentState == PLAYING) {
            // Animation
            animationTimer += deltaTime;
            while (animationTimer >= frameTime) {
                animationTimer -= frameTime;
                currentFrame = (currentFrame + 1) % 4;
                bike.setTextureRect(frameRects[currentFrame]);
            }

            // Boost
            if (isBoosting) {
                boostTimer -= deltaTime;
                bikeY = 450 - 30 * sin(boostTimer * 10.0f);
                
                if (static_cast<int>(boostTimer * 10) % 2 == 0) {
                    bike.setColor(sf::Color(255, 255, 200));
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

            // Puddle effect
            if (inPuddle) {
                puddleSlowTimer -= deltaTime;
                if (puddleSlowTimer <= 0) {
                    bikeSpeed = 4.0f;
                    inPuddle = false;
                    bike.setColor(sf::Color::White);
                }
            }

            // Road
            for (auto& segment : roadSegments) {
                segment.move(0, roadSpeed);
                if (segment.getPosition().y > roadSegmentHeight) {
                    segment.setPosition(0, segment.getPosition().y - 2 * roadSegmentHeight);
                }
            }

            // Obstacles
            for (auto& obstacle : obstacles) {
                obstacle->update(deltaTime);
                
                // Collision
                if (bike.getGlobalBounds().intersects(obstacle->getBounds())) {
                    if (dynamic_cast<PuddleObstacle*>(obstacle.get())) {
                        bikeSpeed = 2.0f;
                        inPuddle = true;
                        puddleSlowTimer = 1.5f;
                        bike.setColor(sf::Color(150, 150, 255));
                    } else {
                        currentState = MENU;
                    }
                }
            }

            // Obstacle management
            if (!obstacles.empty() && obstacles.front()->getY() > 650) {
                obstacles.erase(obstacles.begin());
                addRandomObstacle();
                score += 1 + static_cast<int>(roadSpeed / 3.0f);
                
                if (score % difficultyInterval == 0) {
                    increaseDifficulty();
                }
            }
        }
    }

    void render() {
        window.clear();

        if (currentState == PLAYING) {
            // Road
            for (auto& segment : roadSegments) {
                window.draw(segment);
            }
            
            // Obstacles
            for (auto& obstacle : obstacles) {
                obstacle->draw(window);
            }
            
            // Bike
            window.draw(bike);
            
            // UI
            scoreText.setString("SCORE: " + std::to_string(score));
            window.draw(scoreText);
            
            difficultyText.setString("SPEED: " + std::to_string(static_cast<int>(roadSpeed * 50)) + "%");
            window.draw(difficultyText);
            
            window.draw(boostIcon);
            window.draw(boostText);
        } else {
            // Menu
            window.draw(menuBackground);
            
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                playText.setFillColor(sf::Color(200, 255, 200));
            } else {
                playText.setFillColor(sf::Color::White);
            }
            
            if (quitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                quitText.setFillColor(sf::Color(255, 200, 200));
            } else {
                quitText.setFillColor(sf::Color::White);
            }
            
            window.draw(playText);
            window.draw(quitText);
            
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