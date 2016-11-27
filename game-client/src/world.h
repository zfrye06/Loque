#ifndef LQ_WORLD_H_
#define LQ_WORLD_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "physicalworld.h"
#include "entity.h"

class World {
private:
// vector probably isn't the most efficient way to store the world...
    std::vector<Entity*> entities;
    double stutterLength;
    double stutterPeriod;
public:
    World();
    ~World();
    void addEntity( Entity* e );
    void bringToFront( Entity* e );
    void removeEntity( Entity* e );
    void draw( sf::RenderWindow& window );
    void update( double dt );
    void stutter( double length, double period );
    std::vector<Entity*> getEntitiesByType(Entity::Type t);
};

extern World* world;

#endif
