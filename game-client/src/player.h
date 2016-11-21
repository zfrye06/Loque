#ifndef LQ_PLAYER_H_
#define LQ_PLAYER_H_

#include "entity.h"

static const std::string StateString[] = { "Idle", "Dashing", "Running", "Walking", "JumpSquat", "Airborne", "WallSliding", "SpecialFall" };

class Player : public Entity {
private:
    enum State { Idle, Dashing, Running, Walking, JumpSquat, Airborne, WallSliding, SpecialFall };

    // Configurable variables
    float walkLength;
    float walkTimer;
    float deadZone;
    float dashDifficulty;
    float dashingMultiplier;
    float dashLength;
    float playerHeight;
    float playerWidth;
    float playerSpeed;
    float fullHopHeight;
    float shortHopHeight;
    float airControlMultiplier;

    // Control variables
    int jumps;
    glm::vec2 lastDirection;
    float dashingDirection;
    float dashTimer;

    // Helper variables
    bool onGround;
    State currentState;

    // Helper functions
    void detectGround();
    void setUpBody();
    void setUpSprite( std::string resource );
    void playerIdle( glm::vec2 direction, float dt );
    void playerWalking( glm::vec2 direction, float dt );
    void playerDashing( glm::vec2 direction, float dt);
    void playerRunning( glm::vec2 direction, float dt);

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
