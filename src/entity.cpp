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
    layerOne = new MapLayer( *map, 0 );
}
Map::~Map() {
}
void Map::draw( sf::RenderWindow& window ) {
    window.draw(*layerOne);
}
void Map::update( double dt ) {
}
void Map::onHit( Entity* collider ) {
}
