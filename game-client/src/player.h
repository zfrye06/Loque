#ifndef LQ_PLAYER_H_
#define LQ_PLAYER_H_

#include <glm/glm.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <Box2D/Box2D.h>

#include "entity.h"
#include "playerstate.h"
#include "Animation.hpp"
#include "world.h"
#include "dust.h"
#include "AnimatedSprite.hpp"

class GenericPlayerState;

class Player : public Entity {
public:

    // Configurable variables
    float walkLength;
    float airDodgeVelocity;
    float airDodgeTime;
    float deadZone;
    float turnAroundTime;
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
    float landLength;
    float airControlMultiplier;

    // Control variables
    bool fastFalling;
    bool canDoubleJump;
    bool releasedJump;
    glm::vec2 lastDirection;
    bool jumpButton;
    glm::vec2 direction;
    glm::vec2 position;
    float dashingDirection;
    float dashTimer;
    float jumpSquatTimer;
    bool airDodgePressed;

    // Helper variables
    bool canWallJumpLeft;
    bool canWallJumpRight;
    bool touchingWallLeft;
    bool touchingWallRight;
    bool onGround;
    int controllerID;
    glm::vec2 groundHitNormal;
    float groundAngle;
    GenericPlayerState* currentState;
    GenericPlayerState* newState;

    // Helper functions
    void detectGround();
    void detectWalls();
    float flashPeriod;
    float flashLength;
    float flashTimer;
    sf::Color flashColor;
    void flash(sf::Color c, float length, float period);
    void setUpBody();
    void setUpSounds();
    void setUpSprite( std::string resource );
    void switchState( GenericPlayerState* state );

    bool flipped;

    glm::vec2 smoothCamera;
    sf::View* view;
    sf::Texture* texture;
    b2Body* myBody;
    AnimatedSprite* sprite;
    Animation idleAnimation;
    Animation dashAnimation;
    Animation runAnimation;
    Animation slideAnimation;
    Animation jumpSquatAnimation;
    Animation landingAnimation;
    Animation airborneAnimation;
    Animation airDodgeAnimation;
    Animation jumpingAnimation;
    Animation wallJumpAnimation;
    Animation fallingAnimation;
    Animation specialFallAnimation;
    Animation knockBackAnimation;
    Animation knockBackRecoverAnimation;
    Animation loseAnimation;
    Animation winAnimation;
    Animation shockedAnimation;
    Animation tecAnimation;

    sf::Sound dashSound;
    sf::Sound jump1Sound;
    sf::Sound jump2Sound;
    sf::Sound wallJumpSound;
    sf::Sound airDodgeSound;
    Player( std::string resource, sf::View& view );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderWindow& window );
    Entity::Type getType();
};

#endif
