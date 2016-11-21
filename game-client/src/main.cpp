#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <string>

#include "physicalworld.h"
#include "world.h"
#include "entity.h"
#include "player.h"

int app() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Loque");
    sf::View view;
    view.reset(sf::FloatRect(0,0,800,600));
    World world;
    if(!world.setBackground("assets/images/blueBackground.png")){
        return 1;
    }
    world.addEntity( new Map( "assets/Zapper_Level_1.tmx" ) );
    world.addEntity( new Player( "assets/images/sonicsheet.png", view ) );
    //world.addEntity( new PhysicsDebug(window) );
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
        world.update( dt );
        physicalWorld->step( dt );
        // Actually do rendering.
        window.pushGLStates();
        window.setView( view );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        world.draw(window);
        window.display();
        window.popGLStates();
    }
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
