/**
 * @file map.h
 * @brief Does some complicated mumbo jumbo to read a tmx formatted file, spawn appropriate physics, and layer entities. Doesn't actually draw anything itself.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_MAP_H_
#define LQ_MAP_H_

#include <Box2D/Box2D.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Property.hpp>
#include <SFML/Audio/Music.hpp>

#include "entity.h"
#include "background.h"
#include "exit.h"
#include "laser.h"
#include "lava.h"
#include "SFMLOrthogonalLayer.hpp"
#include "spikes.h"
#include "trampoline.h"
#include "world.h"
#include "layer.h"
#include "killbox.h"
#include "playerspawn.h"

/**
 * @brief Does some complicated mumbo jumbo to read a tmx formatted file, spawn appropriate physics, and layer entities. Doesn't actually draw anything itself.
 */
class Map : public Entity {
private:
    tmx::Map* map;
    bool isPowerOfTwo( int x );
    void spawnObject( tmx::Object& object );
public:
    sf::Music ambient;
    Map( std::string resource );
    ~Map();
    Entity::Type getType();
};

#endif
