/**
 * @file physicsdebug.h
 * @brief Draws debug boxes and stuff so you know what the physics are up to.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_PHYSICSDEBUG_H_
#define LQ_PHYSICSDEBUG_H_

#include "entity.h"
#include "DebugDraw.h"
#include "physicalworld.h"

/**
 * @brief Enables debug rendering for the physics.
 */
class PhysicsDebug : public Entity {
private:
    DebugDraw* drawer;
public:
    PhysicsDebug( sf::RenderTarget& window );
    ~PhysicsDebug();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderTarget& window );
    Entity::Type getType();
};

#endif
