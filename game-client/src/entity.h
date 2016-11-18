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
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include "resource.h"
#include "SFMLOrthogonalLayer.hpp"

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update( double dt );
    virtual void onHit( Entity* collider );
    virtual void draw( sf::RenderWindow& window );
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
    void onHit( Entity* collider );
    void draw( sf::RenderWindow& window );
};

class Player : public Entity {
private:
    float playerSpeed;
    b2Vec2 accel;
    sf::View* view;
    sf::Texture* texture;
    b2BodyDef myBodyDef;
    b2FixtureDef boxFixtureDef;
    b2PolygonShape boxShape;
    b2Body* myBody;
    AnimatedSprite* sprite;
    Animation currentAnimation;
public:
    Player( std::string resource, sf::View& view );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider );
    void draw( sf::RenderWindow& window );
};

#endif
