#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <stdlib.h>

#include "entity.h"
#include "map.h"
#include "physicalworld.h"
#include "player.h"
#include "playerstats.h"
#include "random.h"
#include "background.h"
#include "respawn.h"
#include "world.h"
#include "hud.h"

std::string getMap(unsigned int id ) {
    for(unsigned int i=0;i<MapCount;i++ ) {
        if ( Maps[i].id == id ) {
            return Maps[i].path;
        }
    }
    std::cerr << "Could not find map with id " << id << std::endl;
    exit(1); 
}

int app(int argc, char** argv) {
    if ( argc != 3 ) {
        std::cerr << "Wrong number of arguments\nUsage: ./loque [mapID] [userID]\n";
        return 1;
    }
    Resources = new ResourceManager();
    Random = new RandomClass();
    physicalWorld = new PhysicalWorld();
    sf::View view;
    view.reset(sf::FloatRect(0,0,800,600));
    world = new World(view);
    playerStats = new PlayerStats( 0, 0 );
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Loque");
    world->addEntity( new Map( getMap(std::stoi(argv[1])) ), World::Layer::None );
    world->addEntity( new HUD(), World::Layer::Foreground );
    std::vector<Entity*> spawns = world->getEntitiesByType(Entity::Type::PlayerSpawn);
    if(spawns.size() >= 1){
        ::PlayerSpawn* spawn = static_cast< ::PlayerSpawn*>(spawns[0]);
        world->addEntity(new Respawn(glm::vec2(spawn->pos.x, spawn->pos.y)), World::Layer::Midground);
    }
    //world->addEntity( new PhysicsDebug( world->framebuffer ), World::Layer::Foreground );
    sf::Clock deltaClock;
    // Set up camera view.
    glMatrixMode(GL_MODELVIEW);
    while( window.isOpen() && world->isOpen() ) {
        // Catch events, probably should be in some sort of event handler.
        sf::Event event;
        while( window.pollEvent( event ) ) {
            switch( event.type ) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::Resized: {
                    if ( event.size.height % 2 == 1 ) {
                        event.size.height--;
                    }
                    world->updateView(sf::FloatRect(0,0,event.size.width,event.size.height ));
                    // RenderWindow uses its own matrix mumbojumbo...
                    //glViewport(0, 0, event.size.width, event.size.height);
                    break;
                }
                default: { break; }
            }
        }
        // Update world
        double dt = deltaClock.restart().asSeconds();
        world->update( dt );
        //Pixel align view
        //glm::vec2 center = toGLM(view.getCenter());
        //view.setCenter( round( center.x ), round( center.y ) );
        // Actually do rendering.
        //window.setView( view );
        world->draw(window);
        window.display();
    }
    window.close();
    delete world;
    delete physicalWorld;
    delete Random;
    delete Resources;
    return 0;
}

int main( int argc, char** argv ) {
    try {
        return app(argc, argv);
    } catch( std::exception e ) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
