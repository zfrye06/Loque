#include "world.h"

World* world = new World();

World::~World() {
    for ( Entity* e : entities ) {
        delete e;
    }
}

World::World() {
    stutterLength = 0;
    stutterPeriod = 0;
}

void World::stutter( double length, double period ) {
    stutterLength = length;
    stutterPeriod = period;
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
            entities[i] = nullptr;
            break;
        }
    }
    delete e;
}

void World::draw( sf::RenderWindow& window ) {
    for( int i=0;i<entities.size();i++ ) {
        if ( !entities[i] ) {
            entities.erase( entities.begin() + i );
            i--;
        } else {
            entities[i]->draw(window);
        }
    }
}

void World::update( double dt ) {
    if ( stutterLength > 0 ) {
        stutterLength-=dt;
        if ( fmod(stutterLength,stutterPeriod*2) < stutterPeriod ) {
            return;
        }
    } else {
        stutterLength = 0;
    }
    for( int i=0;i<entities.size();i++ ) {
        if ( !entities[i] ) {
            entities.erase( entities.begin() + i );
            i--;
        } else {
            entities[i]->update(dt);
        }
    }
    physicalWorld->step( dt );
}

std::vector<Entity*> World::getEntitiesByType( Entity::Type t ) {
    std::vector<Entity*> foo;
    for( auto e : entities ) {
        if ( !e ) {
            continue;
        }
        if ( e->getType() == t ) {
            foo.push_back(e);
        }
    }
    return foo;
}
