#ifndef LQ_ENTITY_H_
#define LQ_ENTITY_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Property.hpp>
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
inline b2Vec2 toB2(const glm::vec2 &v) {
        return b2Vec2(v.x, v.y);
}
inline glm::vec2 toGLM(const b2Vec2 &v) {
        return glm::vec2(v.x, v.y);
}

#include "physicalworld.h"
#include "DebugDraw.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include "resource.h"
#include "SFMLOrthogonalLayer.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update( double dt );
    virtual void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    virtual void draw( sf::RenderWindow& window );
    enum class Type{
        Map,
        Player,
        None
    };
    virtual Type getType();
};

class Map : public Entity {
private:
    sf::Music ambient;
    tmx::Map* map;
    std::vector<MapLayer*> layers;
public:
    Map( std::string resource );
    ~Map();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

class PhysicsDebug : public Entity {
private:
    DebugDraw* drawer;
public:
    PhysicsDebug( sf::RenderWindow& window );
    ~PhysicsDebug();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

#endif
