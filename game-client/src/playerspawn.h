/**
 * @file playerspawn.h
 * @brief Literally acts as a point in space that we can find using world->getEntitiesByType(PlayerSpawn)
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */
#ifndef LQ_PLAYERSPAWN_H_
#define LQ_PLAYERSPAWN_H_

#include "entity.h"
#include <tmxlite/Object.hpp>

/**
 * @brief Literally acts as a point in space that we can find using world->getEntitiesByType(PlayerSpawn)
 */
class PlayerSpawn : public Entity {
public:
    glm::vec2 pos;
    PlayerSpawn(tmx::Object& obj);
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
