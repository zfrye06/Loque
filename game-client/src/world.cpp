#include "world.h"

World* world;

World::~World() {
    for( unsigned int l=0;l<LAYERCOUNT;l++ ) {
        for( unsigned int i=0;i<entities[l].size();i++ ) {
            if ( !entities[l][i] ) {
                delete entities[l][i];
            }
        }
    }
}

World::World( sf::View v ) {
    wobble = Resources->getShader("assets/shaders/wobble");
    globalTimer = 0;
    view = v;
    windowView = v;
    timer = 0;
    stutterLength = 0;
    stutterPeriod = 0;
}

void World::updateView( sf::FloatRect r ) {
    if ( (int)r.height % 2 == 1 ) {
        r.height--;
    }
    framebuffer.create( (int)r.width, (int)r.height );
    windowView.reset(r);
    view.reset(r);
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
    for( unsigned int i=0;i<entities[l].size();i++ ) {
        if ( entities[l][i] == e ) {
            entities[l][i] = nullptr;
            delete e;
            return;
        }
    }
    throw std::runtime_error( "Couldn't find entity to delete..." );
}

void World::draw( sf::RenderWindow& window ) {
    view.setCenter( sf::Vector2f( round(view.getCenter().x), round(view.getCenter().y) ));
    framebuffer.setView(view);
    window.setView(windowView);
    //window.setView(view);
    // For each layer, clear, then draw to the frame buffer.
    for( unsigned int l=0;l<LAYERCOUNT;l++ ) {
        framebuffer.clear( sf::Color::Transparent );
        for( unsigned int i=0;i<entities[l].size();i++ ) {
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
            case World::Layer::Lavaground: {
                         sf::Sprite sprite(framebuffer.getTexture());
                         wobble->setUniform("width",(int)window.getSize().x);
                         wobble->setUniform("height",(int)window.getSize().y);
                         wobble->setUniform("texture", sf::Shader::CurrentTexture);
                         wobble->setUniform("time",(float)globalTimer);
                         wobble->setUniform("pos",sf::Glsl::Vec2(sf::Vector2f((float)view.getCenter().x,(float)view.getCenter().y)));
                         window.draw( sprite, wobble );
                         break;
            }
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
    globalTimer += dt;
    timer += dt;
    while ( timer >= TIMESTEP ) {
        for( unsigned int l=0;l<LAYERCOUNT;l++ ) {
            for( unsigned int i=0;i<entities[l].size();i++ ) {
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
    for( unsigned int l=0;l<LAYERCOUNT;l++ ) {
        for( unsigned int i=0;i<entities[l].size();i++ ) {
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
