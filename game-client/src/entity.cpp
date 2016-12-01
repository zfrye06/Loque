#include "entity.h"
#include <vector>
#include <iostream>

Entity::Entity() {
}
Entity::~Entity() {
}
void Entity::update( double dt ) {
}
void Entity::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal  ) {
}
void Entity::draw( sf::RenderTarget& window ) {
}
Entity::Type Entity::getType(){
    return Entity::Type::None;
}
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

PlayerSpawn::PlayerSpawn(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x/64,obj.getPosition().y/64);
}

void PlayerSpawn::update( double dt){
}

void PlayerSpawn::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void PlayerSpawn::draw(sf::RenderTarget& window){
}

Entity::Type PlayerSpawn::getType(){
    return Entity::Type::PlayerSpawn;
}
