/**
 * @file spikes.h
 * @brief Hurts and applies impulse to the player when they touch it.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */
#ifndef LQ_SPIKES_H_
#define LQ_SPIKES_H_

#include "entity.h"
#include "player.h"

/**
 * @brief Hurts and applies impulse to the player when they touch it.
 */
class Spikes : public Entity{
public:
    b2Body* body;
    Spikes(tmx::Object& obj);
    glm::vec2 pos;
    float impulseMultiplier;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
