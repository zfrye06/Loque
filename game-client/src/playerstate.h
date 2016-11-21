#ifndef LQ_PLAYERSTATE_H_
#define LQ_PLAYERSTATE_H_

#include "player.h"

class Player;

static const std::string StateString[] = { "Idle", "Dashing", "Running", "Walking", "JumpSquat", "Airborne", "WallSliding", "SpecialFall", "Jumping", "None" };
enum PlayerState { Idle, Dashing, Running, Walking, JumpSquat, Airborne, WallSliding, SpecialFall, Jumping, None };

class GenericPlayerState {
public:
    Player* player;
    GenericPlayerState( Player* player );
    GenericPlayerState();
    virtual ~GenericPlayerState();
    virtual PlayerState getType();
    virtual void update( Player* player, double dt );
};

class WalkingState : public GenericPlayerState {
public:
    WalkingState( Player* player );
    ~WalkingState();
    PlayerState getType();
    void update( Player* player, double dt );
};

class IdleState : public GenericPlayerState {
public:
    double walkTimer;
    IdleState( Player* player );
    ~IdleState();
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
    PlayerState getType();
    void update( Player* player, double dt );
};

class JumpSquatState : public GenericPlayerState {
public:
    double jumpSquatTimer;
    JumpSquatState( Player* player );
    ~JumpSquatState();
    PlayerState getType();
    void update( Player* player, double dt );
};

class JumpingState : public GenericPlayerState {
public:
    double walkTimer;
    JumpingState( Player* player );
    ~JumpingState();
    PlayerState getType();
    void update( Player* player, double dt );
};

class AirborneState : public GenericPlayerState {
public:
    double walkTimer;
    AirborneState( Player* player );
    ~AirborneState();
    PlayerState getType();
    void update( Player* player, double dt );
};

class RunningState : public GenericPlayerState {
public:
    float dashingDirection;
    RunningState( Player* player, float direction );
    ~RunningState();
    PlayerState getType();
    void update( Player* player, double dt );
};

#endif
