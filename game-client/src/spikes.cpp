#include "spikes.h"

Spikes::Spikes(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x, obj.getPosition().y);

    tmx::FloatRect size = obj.getAABB();
    size.width = size.width / 64;
    size.height = size.height / 64;
    size.left = size.left / 64;
    size.top = size.top / 64;
    if ( size.width > size.height ) {
        impulse = glm::vec2(0.f,-30.f);
    }
    if ( size.height > size.width ) {
        b2AABB testAABB;
        testAABB.lowerBound = b2Vec2(pos.x, pos.y-size.width/2.f);
        testAABB.upperBound = b2Vec2(pos.x+size.width/2.f+0.3, pos.y+size.height/2.f);
        MapQueryCallback queryCallback;
        physicalWorld->get().QueryAABB( &queryCallback, testAABB );
        if( queryCallback.foundMap ) {
            impulse = glm::vec2(30.f,0.f);
        } else {
            impulse = glm::vec2(0.f,30.f);
        }
    }

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
    b2Fixture* fixture = body->CreateFixture( &fixDef );
    fixture->SetSensor(true);
    body->SetUserData( this );
}

void Spikes::update(double dt){
}

void Spikes::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
    if(collider->getType() == Entity::Type::Player){
        ::Player* p = static_cast< ::Player*>(collider);
        if ( !p->isDamageBoosted() ) {
            p->damageBoost();
            p->canDoubleJump = true;
            world->addEntity( new PokeDust( p->position + glm::vec2(0.f,32.f)) );
            world->stutter(p->hitLength/2.f,0.1);
            p->shake(10,p->hitLength,0.1);
            p->switchState( new ShockedState( p, glm::vec2(0.f,-30.f), 1, p->hitLength) );
        }
    }
}

void Spikes::draw(sf::RenderWindow& window){
}

Entity::Type Spikes::getType(){
    return Entity::Type::Spikes;
}
