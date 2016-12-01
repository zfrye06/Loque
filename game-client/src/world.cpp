#include "world.h"

World* world;

World::~World() {
    for( uint l=0;l<LAYERCOUNT;l++ ) {
        for( uint i=0;i<entities[l].size();i++ ) {
            if ( !entities[l][i] ) {
                delete entities[l][i];
            }
        }
    }
}

World::World( sf::View v ) {
    view = v;
    outOfDate = true;
    timer = 0;
    stutterLength = 0;
    stutterPeriod = 0;
}

void World::stutter( double length, double period ) {
    stutterLength = length;
    stutterPeriod = period;
}

void World::addEntity( Entity* e, World::Layer l ) {
    if ( l < 0 || l >= LAYERCOUNT ) {
        throw std::runtime_error( "Something tried to add itself to a non-existant layer! frick!\n" );
    }
    entities[l].push_back(e);
}

void World::removeEntity( Entity* e, World::Layer l ) {
    for( uint i=0;i<entities[l].size();i++ ) {
        if ( entities[l][i] == e ) {
            entities[l][i] = nullptr;
            delete e;
            return;
        }
    }
    throw std::runtime_error( "Couldn't find entity to delete..." );
}

void World::draw( sf::RenderWindow& window ) {
    if ( outOfDate ) {
        framebuffer.create( window.getSize().x, window.getSize().y );
        outOfDate = false;
    }
    framebuffer.setView(view);
    //window.setView(view);
    // For each layer, clear, then draw to the frame buffer.
    for( uint l=0;l<LAYERCOUNT;l++ ) {
        framebuffer.clear( sf::Color::Transparent );
        for( uint i=0;i<entities[l].size();i++ ) {
            if ( !entities[l][i] ) {
                entities[l].erase( entities[l].begin() + i );
                i--;
            } else {
                entities[l][i]->draw(framebuffer);
            }
        }
        // This flushes the framebuffer, drawing everything to it.
        framebuffer.display();
        // Finally depending on the layer, we draw it to the main window.
        switch(l) {
            case World::Layer::None: { break; }
            default: {
                         sf::Sprite sprite(framebuffer.getTexture());
                         window.draw( sprite );
                         break;
                     }
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
    timer += dt;
    while ( timer >= TIMESTEP ) {
        for( uint l=0;l<LAYERCOUNT;l++ ) {
            for( uint i=0;i<entities[l].size();i++ ) {
                if ( !entities[l][i] ) {
                    entities[l].erase( entities[l].begin() + i );
                    i--;
                } else {
                    entities[l][i]->update(TIMESTEP);
                }
            }
        }
        playerStats->update( TIMESTEP );
        physicalWorld->step( TIMESTEP );
        timer -= TIMESTEP;
    }
}

std::vector<Entity*> World::getEntitiesByType( Entity::Type t ) {
    std::vector<Entity*> foo;
    for( uint l=0;l<LAYERCOUNT;l++ ) {
        for( uint i=0;i<entities[l].size();i++ ) {
            if ( !entities[l][i] ) {
                entities[l].erase( entities[l].begin() + i );
                i--;
            } else if ( entities[l][i]->getType() == t ) {
                foo.push_back(entities[l][i]);
            }
        }
    }
    return foo;
}
