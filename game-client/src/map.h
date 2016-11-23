#ifndef LQ_MAP_H_
#define LQ_MAP_H_

#include <Box2D/Box2D.h>
#include <SFML/Audio/Music.hpp>

#include "entity.h"
#include "world.h"
#include "SFMLOrthogonalLayer.hpp"

class Map : public Entity {
private:
    sf::Music ambient;
    tmx::Map* map;
    std::vector<MapLayer*> layers;
    bool isPowerOfTwo( int x );
    void spawnObject( tmx::Object& object );
public:
    Map( std::string resource );
    ~Map();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

#endif
