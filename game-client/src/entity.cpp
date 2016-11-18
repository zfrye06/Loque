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
    playerSpeed = 0.05; // in meters per second
    texture = Resources->getTexture(resource);
    //OK, going to define some animations here...
    currentAnimation.setSpriteSheet(*texture);
    currentAnimation.addFrame(sf::IntRect(69, 193, 68, 93));
    currentAnimation.addFrame(sf::IntRect(0, 0, 70, 96));
    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play( currentAnimation );

    // Physics....
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0, 0);
    myBodyDef.angle = 0;
    myBody = physicalWorld->get().CreateBody( &myBodyDef );
    boxShape.SetAsBox(1,1);
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    myBody->CreateFixture(&boxFixtureDef);
}
Player::~Player() {
    delete sprite;
}
void Player::draw( sf::RenderWindow& window ) {
    window.draw( *sprite );
}
void Player::update( double dt ) {
    sprite->update( sf::seconds(dt) );
    accel = b2Vec2(0,0);
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ) {
        accel += b2Vec2(playerSpeed,0);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ) {
        accel += b2Vec2(-playerSpeed,0);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ) {
        accel += b2Vec2(0,-playerSpeed);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ) {
        accel += b2Vec2(0,playerSpeed);
    }
    //myBody->ApplyForceToCenter( accel, true );
    myBody->ApplyLinearImpulse( accel, b2Vec2(0,0), true );
    b2Vec2 pos = myBody->GetWorldCenter();
    float ang = myBody->GetAngle();
    // We'll assume 64 pixels is a meter
    sprite->setPosition( pos.x*64, pos.y*64 );
    sprite->setRotation( ang );
    //sprite->move( vel );
}
void Player::onHit( Entity* collider ) {
}
