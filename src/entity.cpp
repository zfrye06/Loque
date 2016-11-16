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

Tile::Tile( std::string resource ) {
    sprite.setTexture( *Resources->getTexture( resource.c_str() ) );
}
Tile::~Tile() {
}
void Tile::draw( sf::RenderWindow& window ) {
    window.draw( sprite );
}
void Tile::update( double dt ) {
    sprite.rotate( dt );
}
void Tile::onHit( Entity* collider ) {
}
