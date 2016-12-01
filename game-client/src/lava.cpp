#include "lava.h"

Lava::Lava(tmx::Object& obj){
        
    pos = glm::vec2(obj.getPosition().x, obj.getPosition().y);

    tmx::FloatRect size = obj.getAABB();

    verticies = {
        sf::Vertex(sf::Vector2f(size.left,size.top), sf::Color::Red, sf::Vector2f(0,0)),
        sf::Vertex(sf::Vector2f(size.left+size.width,size.top), sf::Color::Red, sf::Vector2f(1,0)),
        sf::Vertex(sf::Vector2f(size.left+size.width,size.top+size.height), sf::Color::Yellow, sf::Vector2f(1,1)),
        sf::Vertex(sf::Vector2f(size.left,size.top+size.height), sf::Color::Yellow, sf::Vector2f(0,1))
    };

    size.width = size.width / 64;
    size.height = size.height / 64;
    size.left = size.left / 64;
    size.top = size.top / 64;

    impulse = glm::vec2(0.f, -30.f);
    b2Vec2 topLeft(size.left, size.top);
    b2Vec2 topRight(size.left + size.width, size.top);
    b2Vec2 bottomRight(size.left + size.width, size.top + size.height);
    b2Vec2 bottomLeft(size.left, size.top + size.height);

    b2BodyDef boxDef;
    boxDef.type = b2_staticBody;
    boxDef.angle = 0;

    b2PolygonShape shape;
    b2Vec2 vertices[] = {
        topLeft,
        topRight,
        bottomRight,
        bottomLeft
    };
    shape.Set(vertices, 4);
    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.density = 0;
    fixDef.restitution = 0;
    boxDef.position.Set(0,0);
    body = physicalWorld->get().CreateBody( &boxDef );
    body->CreateFixture(&fixDef);
    body->SetUserData( this );
}

void Lava::update(double dt){
}

void Lava::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
    if(collider->getType() == Entity::Type::Player){
        ::Player* p = static_cast< ::Player*>(collider);
        if ( !p->isDamageBoosted() ) {
            playerStats->setScore(playerStats->getScore()-1);
            p->damageBoost();
            p->canDoubleJump = true;
            world->addEntity( new PokeDust( p->position + glm::vec2(0.f,32.f)), World::Layer::Foreground );
            world->stutter(p->hitLength/2.f,0.1);
            p->shake(10,p->hitLength,0.1);
            p->switchState( new ShockedState( p, impulse, 1, p->hitLength) );
        }
    }
}

void Lava::draw(sf::RenderTarget& window){
    window.draw(verticies.data(), verticies.size(), sf::Quads);
}

Entity::Type Lava::getType(){
    return Entity::Type::Lava;
}
