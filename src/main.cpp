#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include "world.h"
#include "entity.h"

int app() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Loque");
    World world;
    world.addEntity( new Map( "assets/candyland.tmx" ) );
    world.addEntity( new Player( "assets/images/alienPink.png" ) );
    sf::Clock deltaClock;
    // Set up camera view.
    sf::View view;
    view.reset(sf::FloatRect(0,0,800,600));
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
        // Actually do rendering.
        window.setView( view );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        world.draw(window);
        window.display();
        // Update world
        world.update( deltaClock.restart().asSeconds() );
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
