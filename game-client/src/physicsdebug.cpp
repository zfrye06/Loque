#include "physicsdebug.h"

PhysicsDebug::PhysicsDebug(sf::RenderTarget& window) {
    uint32 flags = b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;

    drawer = new DebugDraw(window);
    drawer->SetFlags(flags);
    physicalWorld->get().SetDebugDraw(drawer);
}
PhysicsDebug::~PhysicsDebug(){}
void PhysicsDebug::update( double dt ){}
void PhysicsDebug::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal  ){}
void PhysicsDebug::draw( sf::RenderTarget& window ) {
    physicalWorld->get().DrawDebugData();
}

Entity::Type PhysicsDebug::getType(){
    return Entity::Type::None;
}
