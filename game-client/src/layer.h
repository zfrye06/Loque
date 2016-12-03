#ifndef LQ_LAYER_H_
#define LQ_LAYER_H_

#include "entity.h"
#include "player.h"
#include "SFMLOrthogonalLayer.hpp"

class Layer : public Entity {
private:
    MapLayer* mapLayer;
public:
    Layer(MapLayer* ml);
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
