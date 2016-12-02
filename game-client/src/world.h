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

#define TIMESTEP 1.f/75.f
#define LAYERCOUNT 6

class World {
private:
// vector probably isn't the most efficient way to store the world...
    std::vector<Entity*> entities[LAYERCOUNT];
    double timer;
    double globalTimer;
    double stutterLength;
    double stutterPeriod;
    sf::Shader* wobble;
public:
    sf::View windowView;
    sf::View view;
    sf::RenderTexture framebuffer;
    enum Layer { None, Background, Lavaground, AboveLava, Midground, Foreground };
    World( sf::View v);
    ~World();
    void addEntity( Entity* e, World::Layer layer );
    void removeEntity( Entity* e, World::Layer layer );
    void updateView( sf::FloatRect r );
    void draw( sf::RenderWindow& window );
    void update( double dt );
    void stutter( double length, double period );
    std::vector<Entity*> getEntitiesByType(Entity::Type t);
};

extern World* world;

#endif
