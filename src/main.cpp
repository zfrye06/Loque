#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

int main( int argc, char** argv ) {
    sf::Window window(sf::VideoMode(800, 600), "Game");
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
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;
                }
                default: { break; }
            } 
        }
        // Actually do rendering.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        window.display();
    }
}
