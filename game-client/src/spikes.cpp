#include "spikes.h"

Spikes::Spikes(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x, obj.getPosition().y);

    tmx::FloatRect size = obj.getAABB();
    size.width = size.width / 64;
    size.height = size.height / 64;
    size.left = size.left / 64;
    size.top = size.top / 64;

    impulseMultiplier = 30.f;

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
    body->CreateFixture( &fixDef );
    body->SetUserData( this );
}

void Spikes::update(double dt){
}

void Spikes::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
    if(collider->getType() == Entity::Type::Player){
        ::Player* p = static_cast< ::Player*>(collider);
        if ( !p->isDamageBoosted() ) {
            playerStats->setScore(playerStats->getScore()-1);
            p->damageBoost();
            p->canDoubleJump = true;
            world->addEntity( new PokeDust( p->position + glm::vec2(0.f,32.f)), World::Layer::Foreground );
            world->stutter(p->hitLength/2.f,0.1);
            p->shake(10,p->hitLength,0.1);
            p->switchState( new ShockedState( p, glm::normalize(toGLM(hitnormal))*impulseMultiplier, 1, p->hitLength) );
        }
    }
}

void Spikes::draw(sf::RenderTarget& window){
}

Entity::Type Spikes::getType(){
    return Entity::Type::Spikes;
}
