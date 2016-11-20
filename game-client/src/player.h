#ifndef LQ_PLAYER_H_
#define LQ_PLAYER_H_

#include "entity.h"

static const std::string StateString[] = { "Idle", "Dashing", "Running", "Walking", "JumpSquat", "Airborne", "WallSliding", "SpecialFall" };

class Player : public Entity {
private:
    enum State { Idle, Dashing, Running, Walking, JumpSquat, Airborne, WallSliding, SpecialFall };

    // Configurable variables
    float playerHeight;
    float playerWidth;
    float playerSpeed;
    float fullHopHeight;
    float shortHopHeight;
    float airControlMultiplier;

    // Control variables
    int jumps;
    sf::Vector2f lastDirection;

    // Helper variables
    bool onGround;
    State currentState;

    // Helper functions
    void detectGround();
    void setUpBody();
    void setUpSprite( std::string resource );

    b2Vec2 accel;
    sf::View* view;
    sf::Texture* texture;
    b2Body* myBody;
    AnimatedSprite* sprite;
    Animation currentAnimation;
public:
    Player( std::string resource, sf::View& view );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

#endif
