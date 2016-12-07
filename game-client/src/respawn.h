/**
 * @file respawn.h
 * @brief Plays a respawn animation, while forcing the player to watch, and respawns the player at the end of it.
 * @author Zach Frye
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_RESPAWN_H_
#define LQ_RESPAWN_H_

#include <SFML/Audio/Sound.hpp>

#include "AnimatedSprite.hpp"
#include "entity.h"
#include "world.h"
#include "player.h"

/**
 * @brief Plays a respawn animation, while forcing the player to watch, and respawns the player at the end of it.
 */
class Respawn : public Entity{
private:
    sf::Sound sound;
    glm::vec2 position;
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
public:
    Respawn(glm::vec2 pos);
    ~Respawn();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};
#endif
