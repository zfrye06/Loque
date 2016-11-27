#include "world.h"

World* world = new World();

World::~World() {
    for ( Entity* e : entities ) {
        delete e;
    }
}


bool World::setBackground(std::string fileName){
    return background.loadFromFile(fileName);
}

void World::addEntity( Entity* e ) {
    entities.push_back(e);
}

void World::bringToFront( Entity* e ) {
    for( int i=0;i<entities.size();i++ ) {
        if ( entities[i] == e ) {
            std::swap( entities[i], entities[entities.size()-1] );
        }
    }
}

void World::removeEntity( Entity* e ) {
    for( int i=0;i<entities.size();i++ ) {
        if ( entities[i] == e ) {
            entities.erase( entities.begin() + i );
        }
    }
    delete e;
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

std::vector<Entity*> World::getEntitiesByType( Entity::Type t ) {
    std::vector<Entity*> foo;
    for( auto e : entities ) {
        if ( e->getType() == t ) {
            foo.push_back(e);
        }
    }
    return foo;
}
