#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <string>

#include "physicalworld.h"
#include "world.h"
#include "entity.h"
#include "player.h"
#include "map.h"
#include "random.h"
#include "respawn.h"

RandomClass* Random = new RandomClass();

int app() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Loque");
    sf::View view;
    view.reset(sf::FloatRect(0,0,800,600));
    world->addEntity( new Background(&view, "assets/images/sky.png", "assets/images/clouds.png", "assets/images/hills.png" ) );
    world->addEntity( new Map( "assets/Zapper_Level_1.tmx" ) );
    std::vector<Entity*> spawns = world->getEntitiesByType(Entity::Type::PlayerSpawn);
    if(spawns.size() >= 1){
        ::PlayerSpawn* spawn = static_cast< ::PlayerSpawn*>(spawns[0]);
        world->addEntity(new Respawn(glm::vec2(spawn->pos.x, spawn->pos.y), view));
    }
    world->addEntity( new PhysicsDebug( window ) );
    sf::Clock deltaClock;
    // Set up camera view.
    glMatrixMode(GL_MODELVIEW);
    while( window.isOpen() ) {
        std::cout << world->entities.size() << "\n";
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
                    view.reset(sf::FloatRect(0,0,event.size.width,event.size.height ));
                    // RenderWindow uses its own matrix mumbojumbo...
                    //glViewport(0, 0, event.size.width, event.size.height);
                    break;
                }
                case sf::Event::KeyPressed: {
                    if(event.key.code == sf::Keyboard::Escape){
                        ::PlayerSpawn* spawn = static_cast< ::PlayerSpawn*>(spawns[0]);
                        //delete all current respawn animations
                        std::vector<Entity*> anims = world->getEntitiesByType(Entity::Type::RespawnAnim);
                        for(int i = 0; i < anims.size(); i++){
                            world->removeEntity(anims[i]);
                        }
                        world->addEntity(new Respawn(glm::vec2(spawn->pos.x, spawn->pos.y), view));
                        break;
                    }
                }
                default: { break; }
            }
        }
        // Update world
        double dt = deltaClock.restart().asSeconds();
        world->update( dt );
        //Pixel align view
        glm::vec2 center = toGLM(view.getCenter());
        view.setCenter( round( center.x ), round( center.y ) );
        // Actually do rendering.
        window.pushGLStates();
        window.setView( view );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        world->draw(window);
        window.display();
        window.popGLStates();
    }
    delete Resources;
    delete world;
    delete physicalWorld;
    return 0;
}

int main( int argc, char** argv ) {
    try {
        return app();
    } catch( std::exception e ) {
        std::cout << "ERROR: " << e.what() << "\n";
        return 1;
    }
}
