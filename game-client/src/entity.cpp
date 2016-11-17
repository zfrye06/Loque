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

    // Parse map properties
    for ( tmx::Property p : map->getProperties() ) {
        if ( p.getType() == tmx::Property::Type::String && p.getName() == "ambient" ) {
            ambient.openFromFile(p.getStringValue());
            ambient.play();
        }
    }

    background = new MapLayer( *map, 0 );
    ground = new MapLayer( *map, 1 );
}
Map::~Map() {
    ambient.stop();
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
    playerSpeed = 0.08;
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
    sprite->update( sf::seconds(dt) );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ) {
        accel += sf::Vector2f(dt,0);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ) {
        accel += sf::Vector2f(-dt,0);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ) {
        accel += sf::Vector2f(0,-dt);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ) {
        accel += sf::Vector2f(0,dt);
    }
    vel += accel*playerSpeed;
    // Fast acceleration decrease = more responsive movement.
    accel -= accel*(float)dt*50.f;
    // Friction with no input
    vel -= vel*(float)dt*10.f;
    sprite->move( vel );
}
void Player::onHit( Entity* collider ) {
}
