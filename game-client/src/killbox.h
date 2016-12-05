#ifndef LQ_KILLBOX_H_
#define LQ_KILLBOX_H_

#include "entity.h"
#include "player.h"

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
