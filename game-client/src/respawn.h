#ifndef LQ_RESPAWN_H_
#define LQ_RESPAWN_H_

#include <SFML/Audio/Sound.hpp>

#include "AnimatedSprite.hpp"
#include "entity.h"
#include "world.h"
#include "player.h"

class Respawn : public Entity{
private:
    sf::Sound sound;
    glm::vec2 position;
    sf::Texture* texture;
    Animation animation;
    AnimatedSprite* sprite;
    sf::View* view;
public:
    Respawn(glm::vec2 pos, sf::View& view);
    ~Respawn();
    void update(double dt);
    void onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal);
    void draw(sf::RenderWindow& window);
    Entity::Type getType();
};
#endif
