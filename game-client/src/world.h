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
#define LAYERCOUNT 5

class World {
private:
// vector probably isn't the most efficient way to store the world...
    std::vector<Entity*> entities[LAYERCOUNT];
    double timer;
    double stutterLength;
    double stutterPeriod;
    bool outOfDate;
public:
    sf::View view;
    sf::RenderTexture framebuffer;
    enum Layer { None, Background, Midground, Foreground, Lavaground };
    World( sf::View v);
    ~World();
    void addEntity( Entity* e, World::Layer layer );
    void removeEntity( Entity* e, World::Layer layer );
    void draw( sf::RenderWindow& window );
    void update( double dt );
    void stutter( double length, double period );
    std::vector<Entity*> getEntitiesByType(Entity::Type t);
};

extern World* world;

#endif
