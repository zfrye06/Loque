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

    boxDef.type = b2_staticBody;
    boxDef.position.Set(0, 0);
    boxDef.angle = 0;
    boxShape.SetAsBox(.5,.5);
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 500;
    boxFixtureDef.restitution = 0;

    // First grab physical layer
    tmx::TileLayer* tileSet;
    //for( auto l : map->getLayers() ) {
    for ( int i=0;i<map->getLayers().size();i++ ) {
        auto l = map->getLayers()[i].get();
        if ( l->getType() == tmx::Layer::Type::Tile ) {
            if ( l->getName() == "physical" ) {
                tileSet = (tmx::TileLayer*)l;
            }
        }
    }
    // Then loop through the tiles, placing 1x1 boxes where there's tiles.
    auto tiles = tileSet->getTiles();
    auto mapSize = map->getTileCount();
    int x = 0;
    int y = 0;
    for (int i = 0; i < mapSize.x*mapSize.y; i++ ) {
        if ( tiles[i].ID != 0) {
            boxDef.position.Set(x, y);
            b2Body* b = physicalWorld->get().CreateBody( &boxDef );
            b->CreateFixture(&boxFixtureDef);
        }
        x++;
        if ( x >= mapSize.x ) {
            y++;
            x=0;
        }
    }
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

Player::Player( std::string resource, sf::View& view ) {
    this->view = &view;
    playerSpeed = 0.5; // in meters per second
    texture = Resources->getTexture(resource);
    //OK, going to define some animations here...
    currentAnimation.setSpriteSheet(*texture);
    currentAnimation.addFrame(sf::IntRect(69, 193, 68, 93));
    currentAnimation.addFrame(sf::IntRect(0, 0, 70, 96));
    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play( currentAnimation );

    // Physics....
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(4,1);
    myBodyDef.angle = 0;
    myBody = physicalWorld->get().CreateBody( &myBodyDef );
    boxShape.SetAsBox(.5,.3);
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 500;
    boxFixtureDef.restitution = 0;
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
    sprite->setRotation( ang*180/3.149562 );
    view->setCenter( pos.x*64, pos.y*64 );
    //sprite->move( vel );
}
void Player::onHit( Entity* collider ) {
}

PhysicsDebug::PhysicsDebug(sf::RenderWindow& window) {
    uint32 flags = b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;

    drawer = new DebugDraw(window);
    drawer->SetFlags(flags);
    physicalWorld->get().SetDebugDraw(drawer);
}
PhysicsDebug::~PhysicsDebug(){}
void PhysicsDebug::update( double dt ){}
void PhysicsDebug::onHit( Entity* collider ){}
void PhysicsDebug::draw( sf::RenderWindow& window ) {
    window.pushGLStates();
    physicalWorld->get().DrawDebugData();
    window.popGLStates();
}

