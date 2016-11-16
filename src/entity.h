#ifndef LQ_ENTITY_H_
#define LQ_ENTITY_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "resource.h"

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update( double dt );
    virtual void onHit( Entity* collider );
    virtual void draw( sf::RenderWindow& window );
};

class Tile : public Entity {
private:
    sf::Sprite sprite; 
public:
    Tile( std::string resource );
    ~Tile();
    void update( double dt );
    void onHit( Entity* collider );
    void draw( sf::RenderWindow& window );
};

#endif
