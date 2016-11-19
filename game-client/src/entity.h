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

#include "physicalworld.h"
#include "DebugDraw.h"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include "resource.h"
#include "SFMLOrthogonalLayer.hpp"

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update( double dt );
    virtual void onHit( Entity* collider, b2Contact* c );
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
    MapLayer* background;
    MapLayer* ground;
    b2BodyDef boxDef;
    b2PolygonShape boxShape;
    b2FixtureDef boxFixtureDef;
public:
    Map( std::string resource );
    ~Map();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

class Player : public Entity {
private:
    float playerSpeed;
    b2Vec2 accel;
    sf::View* view;
    sf::Texture* texture;
    b2Body* myBody;
    AnimatedSprite* sprite;
    Animation currentAnimation;
public:
    Player( std::string resource, sf::View& view );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c );
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
    void onHit( Entity* collider, b2Contact* c );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

#endif
