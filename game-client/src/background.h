#ifndef LQ_BACKGROUND_H_
#define LQ_BACKGROUND_H_

#include "entity.h"
#include "world.h"

class Background : public Entity {
private:
    sf::Sprite sky;
    sf::Sprite clouds;
    sf::Sprite hills;
public:
    Background(std::string skyFile, std::string cloudsFile, std::string hillsFile);
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
