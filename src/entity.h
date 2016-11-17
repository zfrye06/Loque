#ifndef LQ_ENTITY_H_
#define LQ_ENTITY_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>

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
    tmx::Map* map;
    MapLayer* background;
    MapLayer* ground;
public:
    Map( std::string resource );
    ~Map();
    void update( double dt );
    void onHit( Entity* collider );
    void draw( sf::RenderWindow& window );
};

class Player : public Entity {
private:
    double timer;
    sf::Texture* texture;
    AnimatedSprite* sprite;
    Animation currentAnimation;
public:
    Player( std::string resource );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider );
    void draw( sf::RenderWindow& window );
};

#endif
