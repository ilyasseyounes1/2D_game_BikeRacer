#include "Game.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>

void Game::loadBikeAnimation() {
    if (!bikeSheet.loadFromFile("assets/bike_sheet.png")) {
        sf::Image placeholder;
        placeholder.create(192, 84, sf::Color::Transparent);
        
        for (int i = 0; i < 4; i++) {
            int frameX = i * 48;
            for (int y = 24; y < 60; y++) {
                for (int x = frameX + 12; x < frameX + 36; x++) {
                    placeholder.setPixel(x, y, sf::Color::Red);
                }
            }
            for (int y = 0; y < 84; y++) {
                for (int x = 0; x < 48; x++) {
                    float dist1 = std::hypot(x-(frameX+30), y-72);
                    float dist2 = std::hypot(x-(frameX+18), y-72);
                    if (dist1 < 10 || dist2 < 10) {
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
        frameRects.push_back(sf::IntRect(i * 48, 0, 48, 84));
    }
    
    bike.setTexture(bikeSheet);
    bike.setTextureRect(frameRects[0]);
    bike.setOrigin(bikeWidth/2, bikeHeight/2);
    bikeCollisionBox.width = 30;
    bikeCollisionBox.height = 60;
}

void Game::initRoad() {
    if (!roadTexture.loadFromFile("assets/road_texture.png")) {
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

void Game::resetGame() {
    bike.setPosition(400, 440);
    bikeY = 440.0f;
    bikeRotation = 1.0f;
    obstacles.clear();
    nextObstacleY = -100.0f;
    roadSpeed = 2.5f;
    bikeSpeed = 4.0f;
    gameClock.restart();
    generateInitialObstacles();
    currentFrame = 0;
    animationTimer = 0.0f;
}

void Game::generateInitialObstacles() {
    for (int i = 0; i < 5; ++i) {
        addRandomObstacle();
    }
}

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

void Game::increaseDifficulty() {
    roadSpeed = std::min(maxTrackSpeed, roadSpeed * 1.6f);
}

void Game::setupMenu() {
    playButtonBg.setSize(sf::Vector2f(150, 60));
    playButtonBg.setPosition(325, 245);
    playButtonBg.setFillColor(sf::Color(0, 0, 0, 200));
    playButtonBg.setOutlineThickness(2);
    playButtonBg.setOutlineColor(sf::Color::White);

    aboutButtonBg.setSize(sf::Vector2f(150, 60));
    aboutButtonBg.setPosition(325, 295);
    aboutButtonBg.setFillColor(sf::Color(0, 0, 0, 200));
    aboutButtonBg.setOutlineThickness(2);
    aboutButtonBg.setOutlineColor(sf::Color::White);

    quitButtonBg.setSize(sf::Vector2f(150, 60));
    quitButtonBg.setPosition(325, 345);
    quitButtonBg.setFillColor(sf::Color(0, 0, 0, 200));
    quitButtonBg.setOutlineThickness(2);
    quitButtonBg.setOutlineColor(sf::Color::White);

    playText.setString("PLAY");
    playText.setFont(font);
    playText.setCharacterSize(45);
    playText.setPosition(350, 250);
    playText.setFillColor(sf::Color::White);
    playText.setOutlineColor(sf::Color::Black);
    playText.setOutlineThickness(2);
    playText.setStyle(sf::Text::Bold);

    aboutText.setString("ABOUT");
    aboutText.setFont(font);
    aboutText.setCharacterSize(45);
    aboutText.setPosition(340, 300);
    aboutText.setFillColor(sf::Color::White);
    aboutText.setOutlineColor(sf::Color::Black);
    aboutText.setOutlineThickness(2);
    aboutText.setStyle(sf::Text::Bold);

    quitText.setString("QUIT");
    quitText.setFont(font);
    quitText.setCharacterSize(45);
    quitText.setPosition(350, 350);
    quitText.setFillColor(sf::Color::White);
    quitText.setOutlineColor(sf::Color::Black);
    quitText.setOutlineThickness(2);
    quitText.setStyle(sf::Text::Bold);

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

    difficultyText.setFont(font);
    difficultyText.setCharacterSize(24);
    difficultyText.setPosition(20, 60);
    difficultyText.setFillColor(sf::Color::White);
}

Game::Game() : window(sf::VideoMode(800, 600), "Bike Race") {
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
}

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
        
        elapsedTime = gameClock.getElapsedTime();
        if (static_cast<int>(elapsedTime.asSeconds()) % difficultyInterval == 0) {
            increaseDifficulty();
        }
    }
}

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
        
        difficultyText.setString("SPEED: " + std::to_string(static_cast<int>(roadSpeed * 50)) + "%");
        window.draw(difficultyText);

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
        
        sf::Text title("BIKE RACER", font, 60);
        title.setPosition(220, 100);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(2);
        window.draw(title);
    }
    else if (currentState == ABOUT) {
        window.draw(menuBackground);
        window.draw(aboutPanel);
        window.draw(aboutContentText);
    }

    window.display();
}