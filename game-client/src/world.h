#ifndef LQ_WORLD_H_
#define LQ_WORLD_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "entity.h"

class World {
private:
// vector probably isn't the most efficient way to store the world...
    std::vector<Entity*> entities;
    sf::Texture background;
public:
    bool setBackground(std::string fileName);
    void addEntity( Entity* e );
    void bringToFront( Entity* e );
    void removeEntity( Entity* e );
    void draw( sf::RenderWindow& window );
    void update( double dt );
    std::vector<Entity*> getEntitiesByType(Entity::Type t);
};

extern World* world;

#endif
