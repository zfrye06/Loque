/**
 * @file entity.h
 * @brief The meat and potatoes of a video game, provides easy access to rendering, and a singular contact hook in order to do game logic. Contains some basic entities that have little to no dependencies.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_ENTITY_H_
#define LQ_ENTITY_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Box2D/Box2D.h>

#include <glm/glm.hpp>
// Some math constant stuff, anything that needs GLM is probably going to need these
inline sf::Vector2f toSFML(const glm::vec2 &v) {
        return sf::Vector2f(v.x, v.y);
}
inline b2Vec2 toB2(const glm::vec2 &v) {
        return b2Vec2(v.x, v.y);
}
inline glm::vec2 toGLM(const b2Vec2 &v) {
        return glm::vec2(v.x, v.y);
}
inline glm::vec2 toGLM(const sf::Vector2f &v) {
        return glm::vec2(v.x, v.y);
}

// Imagine using a language that doesn't come with PI by default... OH WAIT
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update( double dt );
    virtual void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    virtual void draw( sf::RenderTarget& window );
    enum Type {
        Map,
        Player,
        PlayerSpawn,
        Laser,
        Trampoline,
        Dust,
        Spikes,
        Lava,
        KillBox,
        Exit,
        RespawnAnim,
        HUD,
        Layer,
        None
    };
    virtual Type getType();
};
#endif
