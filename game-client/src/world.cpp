#include "world.h"

World* world;

World::~World() {
    for ( Entity* e : entities ) {
        delete e;
    }
}

World::World() {
    timer = 0;
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
    for( uint i=0;i<entities.size();i++ ) {
        if ( entities[i] == e ) {
            std::swap( entities[i], entities[entities.size()-1] );
            break;
        }
    }
}

void World::removeEntity( Entity* e ) {
    for( uint i=0;i<entities.size();i++ ) {
        if ( entities[i] == e ) {
            entities[i] = nullptr;
            break;
        }
    }
    delete e;
}

void World::draw( sf::RenderWindow& window ) {
    sf::RenderTexture blah;
    for( uint i=0;i<entities.size();i++ ) {
        if ( !entities[i] ) {
            entities.erase( entities.begin() + i );
            i--;
        } else {
            entities[i]->draw(blah);
        }
        window.draw(blah, &mycoolLavalShader);
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
    timer += dt;
    while ( timer >= TIMESTEP ) {
        for( uint i=0;i<entities.size();i++ ) {
            if ( !entities[i] ) {
                entities.erase( entities.begin() + i );
                i--;
            } else {
                entities[i]->update(TIMESTEP);
            }
        }
        playerStats->update( TIMESTEP );
        physicalWorld->step( TIMESTEP );
        timer -= TIMESTEP;
    }
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
