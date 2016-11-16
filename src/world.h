#ifndef LQ_WORLD_H_
#define LQ_WORLD_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include "entity.h"

class World {
private:
// vector probably isn't the most efficient way to store the world...
    std::vector<Entity*> entities;
public:
    void addEntity( Entity* e );
    void removeEntity( Entity* e );
    void draw( sf::RenderWindow& window );
    void update( double dt );
};

#endif
