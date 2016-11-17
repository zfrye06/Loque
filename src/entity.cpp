#include "entity.h"

Entity::Entity() {
}
Entity::~Entity() {
}
void Entity::update( double dt ) {
}
void Entity::onHit( Entity* collider ) {
}
void Entity::draw( sf::RenderWindow& window ) {
}

Map::Map( std::string resource ) {
    map = Resources->getMap(resource);
    background = new MapLayer( *map, 0 );
    ground = new MapLayer( *map, 1 );
}
Map::~Map() {
    delete background;
    delete ground;
}
void Map::draw( sf::RenderWindow& window ) {
    window.draw(*background);
    window.draw(*ground);
}
void Map::update( double dt ) {
}
void Map::onHit( Entity* collider ) {
}

Player::Player( std::string resource ) {
    timer = 0;
    texture = Resources->getTexture(resource);
    //OK, going to define some animations here...
    currentAnimation.setSpriteSheet(*texture);
    currentAnimation.addFrame(sf::IntRect(69, 193, 68, 93));
    currentAnimation.addFrame(sf::IntRect(0, 0, 70, 96));
    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play( currentAnimation );
}
Player::~Player() {
    delete sprite;
}
void Player::draw( sf::RenderWindow& window ) {
    window.draw( *sprite );
}
void Player::update( double dt ) {
    timer+=dt;
    sprite->update( sf::seconds(dt) );
    sprite->move( sf::Vector2f(sin(timer)/10+1,dt*10) );
}
void Player::onHit( Entity* collider ) {
}
