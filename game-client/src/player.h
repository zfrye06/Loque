#ifndef LQ_PLAYER_H_
#define LQ_PLAYER_H_

#include "entity.h"
#include <glm/glm.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Box2D/Box2D.h>
#include "playerstate.h"
#include "Animation.hpp"
#include "world.h"
#include "AnimatedSprite.hpp"

class GenericPlayerState;

class Player : public Entity {
public:

    // Configurable variables
    float walkLength;
    float deadZone;
    float fastFallSpeed;
    float jumpSquatLength;
    float dashingMultiplier;
    float dashLength;
    float doubleJumpHeight;
    float playerHeight;
    float playerWidth;
    float playerSpeed;
    float jumpHelpAmount;
    float fullHopHeight;
    float shortHopHeight;
    float airControlMultiplier;

    // Control variables
    bool fastFalling;
    bool canDoubleJump;
    bool releasedJump;
    glm::vec2 lastDirection;
    bool jumpButton;
    glm::vec2 direction;
    float dashingDirection;
    float dashTimer;
    float jumpSquatTimer;

    // Helper variables
    bool canWallJumpLeft;
    bool canWallJumpRight;
    bool onGround;
    int controllerID;
    glm::vec2 groundHitNormal;
    GenericPlayerState* currentState;
    GenericPlayerState* newState;

    // Helper functions
    void detectGround();
    void detectWalls();
    void setUpBody();
    void setUpSprite( std::string resource );
    void switchState( GenericPlayerState* state );

    bool flipped;

    sf::View* view;
    sf::Texture* texture;
    b2Body* myBody;
    AnimatedSprite* sprite;
    Animation* currentAnimation;
    Animation walkingAnimation;
    Animation idleAnimation;
    Animation dashingAnimation;
    Animation runningAnimation;
    Animation jumpSquatAnimation;
    Animation airborneAnimation;
    Player( std::string resource, sf::View& view );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

#endif
