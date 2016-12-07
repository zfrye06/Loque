/**
 * @file exit.h
 * @brief On contact with the player, it initiates a victory state.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_EXIT_H_
#define LQ_EXIT_H_

#include "entity.h"
#include "player.h"

/**
 * @brief On contact with the player, it initiates a victory state.
 */
class Exit : public Entity{
public:
    b2Body* body;
    Exit(tmx::Object& obj);
    glm::vec2 pos;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
