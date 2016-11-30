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
<<<<<<< HEAD
// vector probably isn't the most efficient way to store the world...
    std::vector<std::vector<Entity*>> entities;
=======
    std::vector<Entity*> entities;
>>>>>>> a1faef30130a8a922ca7de5e08b9f75345fa82d2
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
