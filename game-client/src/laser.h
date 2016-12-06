#ifndef LQ_LASER_H_
#define LQ_LASER_H_

#include "entity.h"
#include "dust.h"
#include "player.h"
#include "playerstats.h"

class Laser : public Entity {
public:
    float fireworkTimer;
    float fireworkWait;
    sf::Sound sound;
    bool passed;
    b2Body* body;
    Laser(tmx::Object& obj);
    glm::vec2 pos;
    float impulseMultiplier;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
    bool canBePassed;
};

#endif
