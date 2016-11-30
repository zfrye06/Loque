#ifndef LQ_WORLD_H_
#define LQ_WORLD_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "physicalworld.h"
#include "entity.h"
#include "playerstats.h"

#define TIMESTEP 1.f/60.f

class World {
private:
// vector probably isn't the most efficient way to store the world...
    std::vector<std::vector<Entity*>> entities;
    double timer;
    double stutterLength;
    double stutterPeriod;
public:
    enum Layer { Background, Midground, Foreground, Lavaground };
    World();
    ~World();
    void addEntity( Entity* e, int layer );
    void bringToFront( Entity* e );
    void removeEntity( Entity* e );
    void draw( sf::RenderWindow& window );
    void update( double dt );
    void stutter( double length, double period );
    std::vector<Entity*> getEntitiesByType(Entity::Type t);
};

extern World* world;

#endif
