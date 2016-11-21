#include "world.h"

bool World::setBackground(std::string fileName){
    return background.loadFromFile(fileName);
}

void World::addEntity( Entity* e ) {
    entities.push_back(e);
}

void World::removeEntity( Entity* e ) {
    for( int i=0;i<entities.size();i++ ) {
        if ( entities[i] == e ) {
            entities.erase( entities.begin() + i );
        }
    }
}

void World::draw( sf::RenderWindow& window ) {
    for ( Entity* e : entities ) {
        e->draw( window );
    }
}

void World::update( double dt ) {
    for ( Entity* e : entities ) {
        e->update(dt);
    }
}
