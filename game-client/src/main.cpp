#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <string>

#include "physicalworld.h"
#include "world.h"
#include "entity.h"
#include "player.h"
#include "map.h"

int app() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Loque");
    sf::View view;
    view.reset(sf::FloatRect(0,0,800,600));
    world->addEntity( new Background(&view, "assets/images/sky.png", "assets/images/clouds.png", "assets/images/hills.png" ) );
    world->addEntity( new Map( "assets/Zapper_Level_1.tmx" ) );
    world->addEntity( new Player( "assets/images/veemon.png", view ) );
    //world->addEntity( new PhysicsDebug( window ) );
    sf::Clock deltaClock;
    // Set up camera view.
    glMatrixMode(GL_MODELVIEW);
    while( window.isOpen() ) {
        // Catch events, probably should be in some sort of event handler.
        sf::Event event;
        while( window.pollEvent( event ) ) {
            switch( event.type ) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::Resized: {
                    view.reset(sf::FloatRect(0,0,event.size.width,event.size.height ));
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
        physicalWorld->step( dt );
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
