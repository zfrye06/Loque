#ifndef LQ_DASHDUST_H_
#define LQ_DASHDUST_H_

#include <SFML/Audio/Sound.hpp>

#include "AnimatedSprite.hpp"
#include "entity.h"
#include "world.h"
#include "random.h"

class DashDust : public Entity {
private:
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    DashDust( glm::vec2 pos, bool flipped, float angle );
    ~DashDust();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class JumpDust : public Entity {
private:
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    JumpDust( glm::vec2 pos, float angle, bool doub = false );
    ~JumpDust();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class LandingDust : public Entity {
private:
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    LandingDust( glm::vec2 pos, float angle );
    ~LandingDust();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class WallJumpDust : public Entity {
private:
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    WallJumpDust( glm::vec2 pos, bool flipped, float angle );
    ~WallJumpDust();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class ShockDust : public Entity {
private:
    sf::Sound sound;
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    ShockDust( glm::vec2 pos );
    ~ShockDust();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

class PokeDust : public Entity {
private:
    sf::Sound sound;
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    PokeDust( glm::vec2 pos );
    ~PokeDust();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

#endif
