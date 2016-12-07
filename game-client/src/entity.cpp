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
