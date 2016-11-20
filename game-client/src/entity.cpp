#include "entity.h"

Entity::Entity() {
}
Entity::~Entity() {
}
void Entity::update( double dt ) {
}
void Entity::onHit( Entity* collider, b2Contact* c ) {
}
void Entity::draw( sf::RenderWindow& window ) {
}
Entity::Type Entity::getType(){
    return Entity::Type::None;
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
            boxDef.position.Set(x+0.5, y+0.5);
            b2Body* b = physicalWorld->get().CreateBody( &boxDef );
            b->CreateFixture(&boxFixtureDef);
            b->SetUserData(this);
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
void Map::onHit( Entity* collider, b2Contact* c ) {
}
Entity::Type Map::getType(){
    return Entity::Type::Map;
}

Player::Player( std::string resource, sf::View& view ) {
    this->view = &view;
    playerSpeed = 10; // in meters per second
    texture = Resources->getTexture(resource);
    //OK, going to define some animations here...
    currentAnimation.setSpriteSheet(*texture);
    currentAnimation.addFrame(sf::IntRect(69, 193, 68, 93));
    currentAnimation.addFrame(sf::IntRect(0, 0, 70, 96));
    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play( currentAnimation );

    // Physics....
    b2BodyDef myBodyDef;
    myBodyDef.fixedRotation = true;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0,0);
    myBodyDef.angle = 0;
    myBody = physicalWorld->get().CreateBody( &myBodyDef );
    myBody->SetUserData(this);
    b2PolygonShape boxShape;
    b2CircleShape circleShape;
    circleShape.m_p.Set(0,.30);
    circleShape.m_radius = .35;
    boxShape.SetAsBox(.35,.30);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.restitution = 0;
    myBody->CreateFixture(&boxFixtureDef);
    boxFixtureDef.shape = &circleShape;
    myBody->CreateFixture(&boxFixtureDef);
    circleShape.m_p.Set(0,-.30);
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
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right) || sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) {
        accel += b2Vec2(playerSpeed,0);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) || sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) {
        accel += b2Vec2(-playerSpeed,0);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) || sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ) {
        accel += b2Vec2(0,-playerSpeed);
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) || sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) {
        accel += b2Vec2(0,playerSpeed);
    }
    //myBody->ApplyForceToCenter( accel, true );
    b2Vec2 tempAccel( accel.x * dt, accel.y * dt);
    myBody->ApplyLinearImpulse(tempAccel, b2Vec2(.5f, .5f), true );
    b2Vec2 pos = myBody->GetWorldCenter();
    float ang = myBody->GetAngle();
    // We'll assume 64 pixels is a meter
    sprite->setPosition( pos.x*64-35, pos.y*64-50 );
    sprite->setRotation( ang*180/3.149562 );
    view->setCenter( pos.x*64, pos.y*64 );
    //sprite->move( vel );
}
void Player::onHit( Entity* collider, b2Contact* c ) {
}
Entity::Type Player::getType(){
    return Entity::Type::Player;
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
void PhysicsDebug::onHit( Entity* collider, b2Contact* c ){}
void PhysicsDebug::draw( sf::RenderWindow& window ) {
    window.pushGLStates();
    physicalWorld->get().DrawDebugData();
    window.popGLStates();
}

Entity::Type PhysicsDebug::getType(){
    return Entity::Type::None;
}

