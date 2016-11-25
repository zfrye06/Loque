#ifndef LQ_PLAYERSTATE_H_
#define LQ_PLAYERSTATE_H_

#include <tweeny.h>

#include "player.h"

class Player;

static const std::string StateString[] = { "Idle", "Dashing", "Running", "Walking", "JumpSquat", "Airborne", "SpecialFall", "Jumping", "AirDodge", "Turning", "None" };
enum PlayerState { Idle, Dashing, Running, Walking, JumpSquat, Airborne, SpecialFall, Jumping, AirDodge, Turning, None };

class GenericPlayerState {
public:
    Player* player;
    GenericPlayerState( Player* player );
    GenericPlayerState();
    virtual void init();
    virtual ~GenericPlayerState();
    virtual PlayerState getType();
    virtual void update( Player* player, double dt );
};

class WalkingState : public GenericPlayerState {
public:
    WalkingState( Player* player );
    ~WalkingState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class IdleState : public GenericPlayerState {
public:
    double walkTimer;
    bool reset;
    IdleState( Player* player );
    ~IdleState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class DashingState : public GenericPlayerState {
public:
    float dashingDirection;
    double walkTimer;
    double dashTimer;
    DashingState( Player* player, float direction );
    ~DashingState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class JumpSquatState : public GenericPlayerState {
public:
    double jumpSquatTimer;
    JumpSquatState( Player* player );
    ~JumpSquatState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class JumpingState : public GenericPlayerState {
public:
    bool canWallJump;
    double walkTimer;
    float wallJumpDirection;
    JumpingState( Player* player, float wallJumpDirection = 0 );
    ~JumpingState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class AirborneState : public GenericPlayerState {
public:
    bool canWallJump;
    double walkTimer;
    AirborneState( Player* player );
    ~AirborneState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class RunningState : public GenericPlayerState {
public:
    bool crossedNeutral;
    double walkTimer;
    float dashingDirection;
    RunningState( Player* player, float direction );
    ~RunningState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class AirDodgeState : public GenericPlayerState {
    glm::vec2 airDirection;
    glm::vec2 vel;
    tweeny::tween<float> tweenX,tweenY;
public:
    AirDodgeState( Player* player, glm::vec2 airdirection );
    ~AirDodgeState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class SpecialFallState : public GenericPlayerState {
public:
    SpecialFallState( Player* player);
    ~SpecialFallState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

class TurningState : public GenericPlayerState {
    tweeny::tween<float> tweenX;
    float turnDirection;
    float turnTimer;
public:
    TurningState( Player* player, float direction);
    ~TurningState();
    void init();
    PlayerState getType();
    void update( Player* player, double dt );
};

#endif
