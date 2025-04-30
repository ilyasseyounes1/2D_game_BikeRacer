#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
protected:
    sf::Sprite sprite;
    float speed;
    float scale = 1.0f;
    sf::FloatRect collisionBox;
    
public:
    virtual ~Obstacle() = default;
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getCollisionBox() const;
    float getY() const;
    void setPosition(float x, float y);
    void setScale(float s);
    void setCollisionBox(float width, float height);
};

class CarObstacle : public Obstacle {
public:
    CarObstacle(float x, float y, const sf::Texture& texture);
};

class Car2Obstacle : public Obstacle {
public:
    Car2Obstacle(float x, float y, const sf::Texture& texture);
};

class ConstructionObstacle : public Obstacle {
public:
    ConstructionObstacle(float x, float y, const sf::Texture& texture);
};

class RoadBarrierObstacle : public Obstacle {
public:
    RoadBarrierObstacle(float x, float y, const sf::Texture& texture);
};

class ParkedBikeObstacle : public Obstacle {
public:
    ParkedBikeObstacle(float x, float y, const sf::Texture& texture);
};

class PuddleObstacle : public Obstacle {
public:
    PuddleObstacle(float x, float y, const sf::Texture& texture);
};