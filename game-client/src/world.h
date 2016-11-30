#ifndef LQ_WORLD_H_
#define LQ_WORLD_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>
#include "entity.h"
#include "physicalworld.h"
#include "playerstats.h"

#define TIMESTEP 1.f/60.f

class World {
private:
    std::vector<Entity*> entities;
    double timer;
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
