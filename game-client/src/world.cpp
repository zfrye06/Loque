#include "world.h"

World* world;

World::~World() {
    for( unsigned int l=0;l<LAYERCOUNT;l++ ) {
        for( unsigned int i=0;i<entities[l].size();i++ ) {
            if ( entities[l][i] ) {
                delete entities[l][i];
            }
        }
    }
}

World::World( sf::View v ) {
    c = sf::Color(255,255,255,255);
    open = true;
    wobble = Resources->getShader("assets/shaders/wobble");
    blur = Resources->getShader("assets/shaders/blur");
    globalTimer = 0;
    for (int i=0;i<LAYERCOUNT;i++ ) {
        framebuffer[i].create((int) v.getSize().x, (int) v.getSize().y);
    }
	framebuffer[0].setSmooth(true);
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
    for (int i=0;i<LAYERCOUNT;i++ ) {
        framebuffer[i].create( (int)r.width, (int)r.height );
    }
    windowView.reset( r );
    view.reset( r );
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
    std::cerr<<"Couldn't find entity to delete..."<<std::endl;
    throw std::runtime_error( "Couldn't find entity to delete..." );
}

void World::draw( sf::RenderWindow& window ) {
    view.setCenter( sf::Vector2f( round(view.getCenter().x), round(view.getCenter().y) ));
    window.setView( windowView );
    window.clear( sf::Color::Transparent );
    //window.setView(view);
    // For each layer, clear, then draw to the frame buffer.
    for( unsigned int l=1;l<LAYERCOUNT;l++ ) {
        framebuffer[l].setView(view);
        framebuffer[l].clear( sf::Color::Transparent );
        for( unsigned int i=0;i<entities[l].size();i++ ) {
            if ( !entities[l][i] ) {
                entities[l].erase( entities[l].begin() + i );
                i--;
            } else {
                entities[l][i]->draw(framebuffer[l]);
            }
        }
        // This flushes the framebuffer, drawing everything to it.
        framebuffer[l].display();
        // Finally depending on the layer, we draw it to the main window.
        switch(l) {
            case World::Layer::None: { break; }
            case World::Layer::Background: {
						 // Anything higher than one loses quality.
						 float blurAmount = 1;
                         sf::Sprite sprite(framebuffer[l].getTexture());
                         // Going to use the None framebuffer as the horizontal blur
                         framebuffer[0].setView(windowView);
                         framebuffer[0].clear( sf::Color::Transparent );
                         blur->setUniform("texture", sf::Shader::CurrentTexture);
                         blur->setUniform("resolution",sf::Glsl::Vec2(sf::Vector2f((float)windowView.getSize().x,(float)windowView.getSize().y)));
                         blur->setUniform("direction",sf::Glsl::Vec2(sf::Vector2f(blurAmount,0)));
                         framebuffer[0].draw( sprite, blur );
                         // Then we re-draw the horiztonal blur with the vertical blur, to the screen!
						 framebuffer[0].display();
                         sf::Sprite sprite2(framebuffer[0].getTexture());
                         sprite2.setColor( c );
                         blur->setUniform("direction",sf::Glsl::Vec2(sf::Vector2f(0,blurAmount)));
                         window.draw( sprite2, blur );
                         break;
                                           }
            case World::Layer::Lavaground: {
                         sf::Sprite sprite(framebuffer[l].getTexture());
                         wobble->setUniform("width",(int)window.getSize().x);
                         wobble->setUniform("height",(int)window.getSize().y);
                         wobble->setUniform("texture", sf::Shader::CurrentTexture);
                         wobble->setUniform("time",(float)globalTimer);
                         wobble->setUniform("pos",sf::Glsl::Vec2(sf::Vector2f((float)view.getCenter().x,(float)view.getCenter().y)));
                         sprite.setColor( c );
                         window.draw( sprite, wobble );
                         break;
            }
            default: {
                         sf::Sprite sprite(framebuffer[l].getTexture());
                         sprite.setColor( c );
                         window.draw( sprite);
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
    // We want a fixed timestep, this particular while loop
    // can cause a "sprial of doom", but I don't really care
    // for an edge case like that.
    if ( timer >= TIMESTEP ) {
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

void World::close() {
    open = false;
}

bool World::isOpen() {
    return open;
}
