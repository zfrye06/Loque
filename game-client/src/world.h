/**
 * @file world.h
 * @brief Handles storing entities, finding entities, drawing entities, updating entities, and does some framebuffer/view management as well.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_WORLD_H_
#define LQ_WORLD_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

#include "resource.h"
#include "entity.h"
#include "physicalworld.h"
#include "playerstats.h"

// Determines the the target frame rate 
#define TIMESTEP 1.f/60.f
// A badly used constant in order to know how many layers are available.
#define LAYERCOUNT 6

/**
 * @brief Handles storing lists of entities, generating framebuffers, drawing the world, updating the world, finding aspects about the world...
 */
class World {
private:
    std::vector<Entity*> entities[LAYERCOUNT];
    bool open;
    double timer;
    double globalTimer;
    double stutterLength;
    double stutterPeriod;
    sf::Shader* wobble;
    sf::Shader* blur;
public:
    sf::Color c;
    bool isOpen();
    void close();
    sf::View windowView;
    sf::View view;
    sf::RenderTexture framebuffer[LAYERCOUNT];
    enum Layer { None, Background, Lavaground, AboveLava, Midground, Foreground };
    World( sf::View v);
    ~World();
    void addEntity( Entity* e, World::Layer layer );
    void removeEntity( Entity* e, World::Layer layer );
    void updateView( sf::FloatRect r );
    void draw( sf::RenderWindow& window );
    void update( double dt );
    /**
     * @brief Stutters the game in order to add JUICE
     *
     * @param length How long to stutter for.
     * @param period Length of each stutter.
     */
    void stutter( double length, double period );
    std::vector<Entity*> getEntitiesByType(Entity::Type t);
};

extern World* world;

#endif
