#ifndef LQ_LASER_H_
#define LQ_LASER_H_

#include "entity.h"
#include "player.h"
#include "dust.h"

class Laser : public Entity {
public:
    b2Body* body;
    Laser(tmx::Object& obj);
    glm::vec2 pos;
    glm::vec2 impulse;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
    bool canBePassed;
};

#endif
