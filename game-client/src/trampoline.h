/**
 * @file trampoline.h
 * @brief Does what the name implies.
 * @author Zach Frye
 * @version 0.0.0
 * @date 2016-12-06
 */
#ifndef LQ_TRAMPOLINE_H_
#define LQ_TRAMPOLINE_H_

#include "entity.h"
#include "player.h"

/**
 * @brief Does what the name implies.
 */
class Trampoline : public Entity {
public:
    Trampoline(tmx::Object& obj);
    glm::vec2 pos;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
