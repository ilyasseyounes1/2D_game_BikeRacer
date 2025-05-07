#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    virtual ~Obstacle() = default;
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getCollisionBox() const;
    float getY() const;
    void setPosition(float x, float y);
    void setScale(float s);
    void setCollisionBox(float width, float height);

protected:
    sf::Sprite sprite;
    sf::FloatRect collisionBox;
    float speed = 3.0f;
    float scale = 1.0f;
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