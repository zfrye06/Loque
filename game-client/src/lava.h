/**
 * @file lava.h
 * @brief Spawns some sick animated lava, with a separated top layer with sweet bubbling stuffs.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_LAVA_H_
#define LQ_LAVA_H_

#include <array>

#include "entity.h"
#include "world.h"
#include "player.h"

/**
 * @brief Spawns some sick animated lava, with a separated top layer with sweet bubbling stuffs.
 */
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
    float impulseMultiplier;
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
