#ifndef LQ_SPIKES_H_
#define LQ_SPIKES_H_

#include "entity.h"
#include "player.h"

class Spikes : public Entity{
public:
    b2Body* body;
    Spikes(tmx::Object& obj);
    glm::vec2 pos;
    glm::vec2 impulse;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};

#endif
