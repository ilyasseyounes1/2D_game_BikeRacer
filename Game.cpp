#include "Game.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>
    //********************************************************** */
void Game::loadBikeAnimation() {
    bool loadedFromFile = bikeSheet.loadFromFile("assets/bike_sheet.png");

    if (!loadedFromFile) {
        // If loading fails, create a simple placeholder image
        int frameWidth = 48;
        int frameHeight = 84;
        sf::Image placeholder;
        placeholder.create(frameWidth, frameHeight, sf::Color::Red); 
        bikeSheet.loadFromImage(placeholder);
    }

    bikeSheet.setSmooth(false);

    frameRects.clear();
    if (loadedFromFile) {
        for (int i = 0; i < 4; ++i) {
            frameRects.push_back(sf::IntRect(i * 48, 0, 48, 84));
        }
        currentFrame %= 4;
    } else {
        frameRects.push_back(sf::IntRect(0, 0, 48, 84));
        currentFrame = 0;
    }

    bike.setTexture(bikeSheet);
    bike.setTextureRect(frameRects[currentFrame]);

    bike.setOrigin(48.0f / 2.0f, 84.0f / 2.0f);

    bikeCollisionBox.width = 30;
    bikeCollisionBox.height = 60;
}

void Game::initRoad() {
    if (!roadTexture.loadFromFile("assets/road_texture.png")) {
        sf::Image roadImg;
        roadImg.create(800, 600, sf::Color(100, 100, 100)); // Solid dark grey
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
//*********************************************************************** */
void Game::resetGame() {
    bike.setPosition(400, 440);
    bikeY = 440.0f;
    bikeRotation = 1.0f;
    obstacles.clear(); 
    nextObstacleY = -100.0f;
    roadSpeed = 2.5f; 
    bikeSpeed = 4.0f; 
    gameClock.restart(); 

    for (int i = 0; i < 5; ++i) {
        addRandomObstacle();
    }
    currentFrame = 0; 
    animationTimer = 0.0f; }


void Game::addRandomObstacle() {
    float obstacleX = 100 + rand() % 600;

    int obstacleType = rand() % 5;
    switch (obstacleType) {
        case 0: obstacles.push_back(std::make_unique<CarObstacle>(obstacleX, nextObstacleY, carTexture)); break;
        case 1: obstacles.push_back(std::make_unique<Car2Obstacle>(obstacleX, nextObstacleY, car2Texture)); break;
        case 2: obstacles.push_back(std::make_unique<ConstructionObstacle>(obstacleX, nextObstacleY, constructionTexture)); break;
        case 3: obstacles.push_back(std::make_unique<RoadBarrierObstacle>(obstacleX, nextObstacleY, barrierTexture)); break;
        case 4: obstacles.push_back(std::make_unique<ParkedBikeObstacle>(obstacleX, nextObstacleY, parkedBikeTexture)); break;
    }
    nextObstacleY -= obstacleSpacing;
}
//**************************************************************** */

void Game::setupButtonBackground(sf::RectangleShape& shape, const sf::Vector2f& position) {
    shape.setSize(sf::Vector2f(150, 60));
    shape.setPosition(position);
    shape.setFillColor(sf::Color(0, 0, 0, 200));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);
}

void Game::setupButtonText(sf::Text& text, const std::string& str, const sf::Vector2f& position) {
    text.setString(str);
    text.setFont(font);
    text.setCharacterSize(45);
    text.setPosition(position);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setStyle(sf::Text::Bold);
}

void Game::setupMenu() {
    setupButtonBackground(playButtonBg, sf::Vector2f(325, 245));
    setupButtonBackground(aboutButtonBg, sf::Vector2f(325, 295));
    setupButtonBackground(quitButtonBg, sf::Vector2f(325, 345));

    setupButtonText(playText, "PLAY", sf::Vector2f(350, 250));
    setupButtonText(aboutText, "ABOUT", sf::Vector2f(340, 300));
    setupButtonText(quitText, "QUIT", sf::Vector2f(350, 350));

    aboutPanel.setSize(sf::Vector2f(600, 400));
    aboutPanel.setPosition(100, 100);
    aboutPanel.setFillColor(sf::Color(0, 0, 0, 220));
    aboutPanel.setOutlineThickness(3);
    aboutPanel.setOutlineColor(sf::Color::White);

    aboutContentText.setString("BIKE RACER GAME\n\n"
                               "Avoid obstacles!\n"
                               "Controls:\n"
                               "LEFT/RIGHT - Move\n"
                               "ESC - Pause\n\n"
                               "Created with SFML");
    aboutContentText.setFont(font);
    aboutContentText.setCharacterSize(30);
    aboutContentText.setPosition(150, 150);
    aboutContentText.setFillColor(sf::Color::White);

    pauseText.setString("PAUSED\nPress ESC to resume");
    pauseText.setFont(font);
    pauseText.setCharacterSize(50);
    pauseText.setPosition(200, 200);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setStyle(sf::Text::Bold);

    timeText.setFont(font);
    timeText.setCharacterSize(28);
    timeText.setPosition(20, 20);
    timeText.setFillColor(sf::Color::White);
    timeText.setStyle(sf::Text::Bold);

    } 
//************************************************************************************* */
/*Game::Game() : window(sf::VideoMode(800, 600), "Bike Race") {
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    
    std::srand(std::time(nullptr));
    
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
    
    if (!carTexture.loadFromFile("assets/car.jpg")) {
        sf::Image carImg;
        carImg.create(100, 60, sf::Color::Transparent);
        for (int y = 20; y < 40; y++) {
            for (int x = 10; x < 90; x++) carImg.setPixel(x, y, sf::Color::Blue);
        }
        carTexture.loadFromImage(carImg);
    }
    
    if (!car2Texture.loadFromFile("assets/car2.png")) {
        sf::Image carImg;
        carImg.create(100, 60, sf::Color::Transparent);
        for (int y = 20; y < 40; y++) {
            for (int x = 10; x < 90; x++) carImg.setPixel(x, y, sf::Color::Red);
        }
        car2Texture.loadFromImage(carImg);
    }
    
    if (!constructionTexture.loadFromFile("assets/construction.png")) {
        sf::Image conImg;
        conImg.create(100, 100, sf::Color::Transparent);
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
    
    if (!parkedBikeTexture.loadFromFile("assets/parked_bike.png")) {
        sf::Image bikeImg;
        bikeImg.create(200, 67, sf::Color::Transparent);
        for (int y = 20; y < 50; y++) {
            for (int x = 50; x < 150; x++) bikeImg.setPixel(x, y, sf::Color::Blue);
        }
        for (int y = 0; y < 67; y++) {
            for (int x = 0; x < 200; x++) {
                if (std::hypot(x-60, y-60) < 15 || std::hypot(x-140, y-60) < 15) {
                    bikeImg.setPixel(x, y, sf::Color::Black);
                }
            }
        }
        parkedBikeTexture.loadFromImage(bikeImg);
    }

    background.setTexture(backgroundTexture);
    menuBackground.setTexture(menuBgTexture);
    
    loadBikeAnimation();
    initRoad();
    resetGame();

    if (!font.loadFromFile("assets/arial.ttf")) {
        font = sf::Font();
    }
    setupMenu();
}  */


bool Game::loadTextureWithSimplePlaceholder(sf::Texture& texture, const std::string& filePath,
    unsigned int placeholderWidth, unsigned int placeholderHeight,
    const sf::Color& placeholderColor) {
if (!texture.loadFromFile(filePath)) {
sf::Image placeholder;
placeholder.create(placeholderWidth, placeholderHeight, placeholderColor);
texture.loadFromImage(placeholder);
return false; 
}
return true; 
}


Game::Game() : window(sf::VideoMode(800, 600), "Bike Race") {
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    std::srand(std::time(nullptr));

    // Use the helper function for texture loading
    loadTextureWithSimplePlaceholder(backgroundTexture, "assets/background.png", 800, 600, sf::Color(135, 206, 235)); // Sky blue fallback
    loadTextureWithSimplePlaceholder(menuBgTexture, "assets/menu_bg.png", 800, 600, sf::Color(50, 50, 100)); // Dark blue/grey fallback

    loadTextureWithSimplePlaceholder(carTexture, "assets/car.jpg", 100, 60, sf::Color::Blue);
    loadTextureWithSimplePlaceholder(car2Texture, "assets/car2.png", 100, 60, sf::Color::Red);
    loadTextureWithSimplePlaceholder(constructionTexture, "assets/construction.png", 100, 100, sf::Color(255, 165, 0)); // Orange fallback
    loadTextureWithSimplePlaceholder(barrierTexture, "assets/barrier.png", 120, 30, sf::Color::Red);
    loadTextureWithSimplePlaceholder(parkedBikeTexture, "assets/parked_bike.png", 200, 67, sf::Color::Blue);


    background.setTexture(backgroundTexture);
    menuBackground.setTexture(menuBgTexture);

    loadBikeAnimation(); 
    initRoad();          
    resetGame();

    if (!font.loadFromFile("assets/arial.ttf")) {
        font = sf::Font(); 
    }

    setupMenu(); 

}
//***************************************************************************** */
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleInput();
        if (currentState == PLAYING) {
            update(deltaTime);
        }
        render();
    }
}
//******************************************************************************** */
void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (currentState == PLAYING) {
                currentState = PAUSED;
            } else if (currentState == PAUSED) {
                currentState = PLAYING;
            } else if (currentState == ABOUT) {
                currentState = MENU;
            }
        }

        if (currentState == MENU && event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            if (playText.getGlobalBounds().contains(mousePos)) {
                currentState = PLAYING;
                resetGame();
            }
            else if (aboutText.getGlobalBounds().contains(mousePos)) {
                currentState = ABOUT;
            }
            else if (quitText.getGlobalBounds().contains(mousePos)) {
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
        bikeCollisionBox.left = bikeX - bikeCollisionBox.width/2;
        bikeCollisionBox.top = bikeY - bikeCollisionBox.height/2;
    }
}
//******************************************************************************************** */
void Game::update(float deltaTime) {
    animationTimer += deltaTime;
    while (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        currentFrame = (currentFrame + 1) % 4;
        bike.setTextureRect(frameRects[currentFrame]);
    }

    for (auto& segment : roadSegments) {
        segment.move(0, roadSpeed);
        if (segment.getPosition().y > roadSegmentHeight) {
            segment.setPosition(0, segment.getPosition().y - 2 * roadSegmentHeight);
        }
    }

    for (auto& obstacle : obstacles) {
        obstacle->update(deltaTime);
        
        if (bikeCollisionBox.intersects(obstacle->getCollisionBox())) {
            currentState = MENU;
        }
    }

    if (!obstacles.empty() && obstacles.front()->getY() > 650) {
        obstacles.erase(obstacles.begin());
        addRandomObstacle();
    }
}
//********************************************************************************************** */
void Game::render() {
    window.clear();

    if (currentState == PLAYING || currentState == PAUSED) {
        for (auto& segment : roadSegments) {
            window.draw(segment);
        }
        
        for (auto& obstacle : obstacles) {
            obstacle->draw(window);
        }
        
        window.draw(bike);
        
        elapsedTime = gameClock.getElapsedTime();
        int seconds = static_cast<int>(elapsedTime.asSeconds());
        int minutes = seconds / 60;
        seconds %= 60;
        timeText.setString("TIME: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
        window.draw(timeText);
       

        if (currentState == PAUSED) {
            window.draw(pauseText);
        }
    } 
    else if (currentState == MENU) {
        window.draw(menuBackground);
        window.draw(playButtonBg);
        window.draw(aboutButtonBg);
        window.draw(quitButtonBg);
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (playText.getGlobalBounds().contains(mousePos)) {
            playButtonBg.setFillColor(sf::Color(0, 0, 0, 150));
            playText.setFillColor(sf::Color(200, 255, 200));
        } else {
            playButtonBg.setFillColor(sf::Color(0, 0, 0, 200));
            playText.setFillColor(sf::Color::White);
        }
        
        if (aboutText.getGlobalBounds().contains(mousePos)) {
            aboutButtonBg.setFillColor(sf::Color(0, 0, 0, 150));
            aboutText.setFillColor(sf::Color(200, 200, 255));
        } else {
            aboutButtonBg.setFillColor(sf::Color(0, 0, 0, 200));
            aboutText.setFillColor(sf::Color::White);
        }
        
        if (quitText.getGlobalBounds().contains(mousePos)) {
            quitButtonBg.setFillColor(sf::Color(0, 0, 0, 150));
            quitText.setFillColor(sf::Color(255, 200, 200));
        } else {
            quitButtonBg.setFillColor(sf::Color(0, 0, 0, 200));
            quitText.setFillColor(sf::Color::White);
        }
        
        window.draw(playText);
        window.draw(aboutText);
        window.draw(quitText);
        
      
    }
    else if (currentState == ABOUT) {
        window.draw(menuBackground);
        window.draw(aboutPanel);
        window.draw(aboutContentText);
    }

    window.display();
}