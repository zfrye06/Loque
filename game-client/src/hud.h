#ifndef LQ_HUD_H_
#define LQ_HUD_H_

#include "entity.h"
#include "playerstats.h"
#include "world.h"
#include "player.h"
#include <sstream>

class HUD : public Entity {
public:
    HUD();
    sf::Text scoreText;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
