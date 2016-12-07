/**
 * @file killbox.h
 * @brief Unfortunately doesn't burst the player into gore or anything (i was thinking about it), but it does respawn the player when they touch it.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_KILLBOX_H_
#define LQ_KILLBOX_H_

#include "entity.h"
#include "player.h"

/**
 * @brief Unfortunately doesn't burst the player into gore or anything (i was thinking about it), but it does respawn the player when they touch it.
 */
class KillBox : public Entity{
public:
    b2Body* body;
    KillBox(tmx::Object& obj);
    glm::vec2 pos;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
