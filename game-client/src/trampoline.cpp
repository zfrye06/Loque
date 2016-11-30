#include "trampoline.h"

Trampoline::Trampoline(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x/64, obj.getPosition().y/64);

    tmx::FloatRect size = obj.getAABB();
    size.width = size.width / 64;
    size.height = size.height / 64;
    size.left = size.left / 64;
    size.top = size.top / 64;

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
    b2Body* body = physicalWorld->get().CreateBody( &boxDef );
    b2Fixture* fixture = body->CreateFixture( &fixDef );
    fixture->SetSensor(true);
    body->SetUserData(this);
}

void Trampoline::update(double dt){
}

void Trampoline::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
    if ( collider->getType() == Entity::Type::Player ) {
        ::Player* p = (::Player*)collider;
        if ( p->currentState->getType() == PlayerState::Airborne ) {
            p->myBody->SetLinearVelocity(b2Vec2(p->myBody->GetLinearVelocity().x, -20));
            p->switchState( new JumpingState( p ) );
        }
    }
}

void Trampoline::draw(sf::RenderTarget& window){
}

Entity::Type Trampoline::getType(){
    return Entity::Type::Trampoline;
}
