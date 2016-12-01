#ifndef LQ_LAVA_H_
#define LQ_LAVA_H_

#include <array>

#include "entity.h"
#include "world.h"
#include "player.h"

class Lava : public Entity{
private:
    tmx::FloatRect rect;
    Animation animation;
    AnimatedSprite* sprite;
    sf::Texture* texture;
    Animation surfaceAnimation;
    AnimatedSprite* surfaceSprite;
    sf::Texture* surfaceTexture;
public:
    b2Body* body;
    std::array<sf::Vertex, 4u> verticies;
    Lava(tmx::Object& obj);
    ~Lava();
    glm::vec2 pos;
    glm::vec2 impulse;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
