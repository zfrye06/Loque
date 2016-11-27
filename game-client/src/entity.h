#ifndef LQ_ENTITY_H_
#define LQ_ENTITY_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/glm.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Property.hpp>
#include <Box2D/Box2D.h>
inline sf::Vector2f toSFML(const glm::vec2 &v) {
        return sf::Vector2f(v.x, v.y);
}
inline b2Vec2 toB2(const glm::vec2 &v) {
        return b2Vec2(v.x, v.y);
}
inline glm::vec2 toGLM(const b2Vec2 &v) {
        return glm::vec2(v.x, v.y);
}
inline glm::vec2 toGLM(const sf::Vector2f &v) {
        return glm::vec2(v.x, v.y);
}

#include "physicalworld.h"
#include "AnimatedSprite.hpp"
#include "DebugDraw.h"
#include "resource.h"

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
    enum Type {
        Map,
        Player,
        PlayerSpawn,
        Laser,
        Trampoline,
        Dust,
        None
    };
    virtual Type getType();
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

class Background : public Entity {
private:
    sf::Sprite sky;
    sf::Sprite clouds;
    sf::Sprite hills;
    sf::View* defaultView;
public:
    Background(sf::View* view, std::string skyFile, std::string cloudsFile, std::string hillsFile);
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class PlayerSpawn : public Entity {
public:
    glm::vec2 pos;
    PlayerSpawn(tmx::Object& obj);
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class Laser : public Entity {
public:
    Laser(tmx::Object& obj);
    glm::vec2 pos;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
    bool canBePassed;
};

#endif
