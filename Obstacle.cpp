#include "Obstacle.h"

void Obstacle::update(float deltaTime) {
    sprite.move(0, speed * deltaTime * 60.0f);
    collisionBox.left = sprite.getPosition().x - collisionBox.width/2;
    collisionBox.top = sprite.getPosition().y - collisionBox.height/2;
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Obstacle::getCollisionBox() const {
    return collisionBox;
}

float Obstacle::getY() const {
    return sprite.getPosition().y;
}

void Obstacle::setPosition(float x, float y) { 
    sprite.setPosition(x, y); 
    collisionBox.left = x - collisionBox.width/2;
    collisionBox.top = y - collisionBox.height/2;
}

void Obstacle::setScale(float s) { 
    scale = s; 
    sprite.setScale(scale, scale); 
    collisionBox.width *= s/scale;
    collisionBox.height *= s/scale;
}

void Obstacle::setCollisionBox(float width, float height) {
    collisionBox.width = width;
    collisionBox.height = height;
}

CarObstacle::CarObstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    setScale(0.85f);
    speed = 3.0f;
    setCollisionBox(70, 40);
    sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
}

Car2Obstacle::Car2Obstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    setScale(0.85f);
    speed = 3.0f;
    setCollisionBox(70, 40);
    sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
}

ConstructionObstacle::ConstructionObstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    setScale(1.0f);
    speed = 3.0f;
    setCollisionBox(60, 60);
    sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
}

RoadBarrierObstacle::RoadBarrierObstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    setScale(1.0f);
    speed = 3.0f;
    setCollisionBox(80, 20);
    sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
}

ParkedBikeObstacle::ParkedBikeObstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    setScale(0.6f);
    speed = 3.0f;
    setCollisionBox(60, 30);
    sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
}

PuddleObstacle::PuddleObstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    setScale(0.9f);
    speed = 3.0f;
    setCollisionBox(70, 20);
    sprite.setOrigin(texture.getSize().x/2, texture.getSize().y/2);
}