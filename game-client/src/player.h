#ifndef LQ_PLAYER_H_
#define LQ_PLAYER_H_

#include <Box2D/Box2D.h>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Main.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "AnimatedSprite.hpp"
#include "Animation.hpp"
#include "dust.h"
#include "entity.h"
#include "playerstate.h"
#include "random.h"
#include "world.h"

class GenericPlayerState;

class Player : public Entity {
public:

    // Configurable variables
    float directionalInfluence;
    float hitLength;
    float techLength;
    float frickedUpLength;
    float frickedUpWallJumpLength;
    float damageBoostLength;
    float shockLength;
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
    bool successfulTech;
    float techTimer;
    float frickedUpTimer;
    float damageBoostTimer;
    bool fastFalling;
    bool canDoubleJump;
    float shakePeriod;
    float shakeLength;
    float shakeStrength;
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
    bool touchingCeiling;
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
    bool isDamageBoosted();
    void damageBoost();
    float flashPeriod;
    float flashLength;
    float flashTimer;
    sf::Color flashColor;
    void flash(sf::Color c, float length, float period);
    void shake(float strength, float length, float period);
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
    Animation winRepeatAnimation;
    Animation shockedAnimation;
    Animation tecAnimation;
    Animation hitAnimation;

    sf::Sound hurtSound;
    sf::Sound dashSound;
    sf::Sound jump1Sound;
    sf::Sound jump2Sound;
    sf::Sound wallJumpSound;
    sf::Sound airDodgeSound;
    sf::Sound techSound;
    Player( std::string resource, glm::vec2 pos, sf::View& view );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderTarget& window );
    Entity::Type getType();
};

#endif
