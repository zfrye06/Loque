/**
 * @file player.h
 * @brief Oh man, this abomination of a state machine has the majority of the game logic inside of it. I didn't clearly map out the ownership of variables, or how the state machine would look. Just kinda went crazy. Works great though!
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

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
#include "respawn.h"

class GenericPlayerState;

/**
 * @brief Oh man, this abomination of a state machine has the majority of the game logic inside of it. I didn't clearly map out the ownership of variables, or how the state machine would look. Just kinda went crazy. Works great though!
 */
class Player : public Entity {
public:

    // Configurable variables
    /**
     * @brief also known as hitstun, how long in seconds to be stunned for during getting hit. (not shocked!)
     */
    float hitLength;
    /**
     * @brief How much time in seconds does the game wait before signaling a "failed" breakfall. The higher the easier it is to break your fall from a hit.
     */
    float techLength;
    /**
     * @brief This is how long the game waits before allowing the initiation of another tech. Don't frick up!
     */
    float frickedUpLength;
    /**
     * @brief How long the game waits before allowing the player to be hurt again.
     */
    float damageBoostLength;
    /**
     * @brief How long in seconds for the player to be in hitstun from a shock.
     */
    float shockLength;
    /**
     * @brief The time in seconds the game waits for the stick to be "smashed" before initiating a walk. If the stick was moved fast enough, it initiates a dash instead. Lower = harder to dash.
     */
    float walkLength;
    /**
     * @brief Initial velocity to launch the player at during an air dodge. It's quadratically interpolated to 0 over airDodgeTime.
     */
    float airDodgeVelocity;
    /**
     * @brief How long does an airdodge take place.
     */
    float airDodgeTime;
    /**
     * @brief The deadzone of a joystick, defaults to 0.25 (25%)
     */
    float deadZone;
    /**
     * @brief How long the player takes to turn around during a run, should never be relevant to good players.
     */
    float turnAroundTime;
    /**
     * @brief The amount of force to apply to the player while air-borne to make them enter a fast-falling status.
     */
    float fastFallSpeed;
    /**
     * @brief How long to wait during a jump-squat. If the player releases the jump button during the jumpsquat they will short hop instead of jump.
     */
    float jumpSquatLength;
    /**
     * @brief How much faster is dashing/running from walking.
     */
    float dashingMultiplier;
    /**
     * @brief How long is the player in a dash, during a dash a player may "dash dance", provided they don't wait longer than this time before turning around.
     */
    float dashLength;
    /**
     * @brief What to set the player's y velocity at during a double jump, it's also used in wall jumping calculations.
     */
    float doubleJumpHeight;
    /**
     * @brief Not the true player height, but it's used to generate the player capsule and calculate physics.
     */
    float playerHeight;
    /**
     * @brief The player width.
     */
    float playerWidth;
    /**
     * @brief How fast the player moves while walking or in special fall.
     */
    float playerSpeed;
    /**
     * @brief If the player holds the jump button while jumping, this force is applied upward for the duration.
     */
    float jumpHelpAmount;
    /**
     * @brief The y velocity to set the player at for a full jump.
     */
    float fullHopHeight;
    /**
     * @brief This is a wall jump buffer, if the player inputs a jump too early, as long as they hit a wall within this timeframe they'll still successfully walljump.
     */
    float wallJumpLength;
    /**
     * @brief What to set the player's y velocity at during a short hop.
     */
    float shortHopHeight;
    /**
     * @brief How long does a landing animation take when the player hits the ground. The player should be able to buffer inputs during it, but can't actually act during this time.
     */
    float landLength;
    /**
     * @brief How much air control does the player have.
     */
    float airControlMultiplier;

    // Control variables
    bool airDodgeReleased;
    bool dead;
    bool successfulWallJump;
    float wallJumpDirection;
    bool resetToNeutral;
    bool successfulTech;
    float techTimer;
    float wallJumpTimer;
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
    void setUpSprite( std::string& resource );
    void switchState( GenericPlayerState* state );

    bool flipped;

    glm::vec2 spawnLoc;
    glm::vec2 smoothCamera;
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
    sf::Sound winSound;
    Player( std::string resource, glm::vec2 pos );
    ~Player();
    void update( double dt );
    void onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal );
    void draw( sf::RenderTarget& window );
    Entity::Type getType();
};

#endif
