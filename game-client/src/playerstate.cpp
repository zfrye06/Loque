#include "playerstate.h"

GenericPlayerState::GenericPlayerState( Player* player ) {
    this->player = player;
}
GenericPlayerState::GenericPlayerState() {
}
GenericPlayerState::~GenericPlayerState() {
}
PlayerState GenericPlayerState::getType() {
    return PlayerState::None;
}
void GenericPlayerState::update( Player* player, double dt ) {
}

WalkingState::WalkingState( Player* player ) {
    this->player = player;
}
WalkingState::~WalkingState() {
}
PlayerState WalkingState::getType() {
    return PlayerState::Walking;
}
void WalkingState::update( Player* player, double dt ) {
    glm::vec2 vel = player->direction.x*player->playerSpeed*glm::rotate( player->groundHitNormal, (float)M_PI/2.f );
    player->myBody->SetLinearVelocity( toB2(vel) );

    if ( vel.x == 0 ) {
        player->switchState( new IdleState(player) );
    }
    if ( !player->onGround ) {
        player->switchState( new AirborneState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->releasedJump ) {
            player->switchState( new JumpSquatState(player) );
        }
    } else {
        player->releasedJump = true;
    }
}

IdleState::IdleState( Player* player ) {
    this->player = player;
    player->sprite->play( player->idleAnimation );
    player->sprite->setFrameTime(sf::seconds(0.2));
    walkTimer = 0;
}
IdleState::~IdleState() {
}
PlayerState IdleState::getType() {
    return PlayerState::Idle;
}
void IdleState::update( Player* player, double dt ) {
    if ( player->direction.x != 0 ) {
        walkTimer += dt;
    } else {
        walkTimer = 0;
    }
    if ( walkTimer > player->walkLength ) {
        if ( fabs(player->direction.x) > 0.9 ) {
            if ( player->direction.x > 0 ) {
                player->switchState( new DashingState(player,1) );
            } else {
                player->switchState( new DashingState(player,-1) );
            }
        } else {
            player->switchState( new WalkingState(player) );
        }
    }
    if ( !player->onGround ) {
        player->switchState( new AirborneState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->releasedJump ) {
            player->switchState( new JumpSquatState(player) );
        }
    } else {
        player->releasedJump = true;
    }
}

DashingState::DashingState( Player* player, float dashingDirection ) {
    this->player = player;
    this->dashingDirection = dashingDirection;
    dashTimer = 0;
    walkTimer = 0;
}
DashingState::~DashingState() {
}
PlayerState DashingState::getType() {
    return PlayerState::Dashing;
}
void DashingState::update( Player* player, double dt ) {
    player->sprite->play( player->dashingAnimation );
    player->sprite->setFrameTime(sf::seconds(player->dashLength/(float)player->dashingAnimation.getSize()));
    if ( fabs(player->direction.x) < 0.9 ) {
        walkTimer += dt;
    } else {
        walkTimer = 0;
    }
    glm::vec2 vel = dashingDirection*player->playerSpeed*player->dashingMultiplier*glm::rotate( player->groundHitNormal, (float)M_PI/2.f );
    player->myBody->SetLinearVelocity( toB2(vel) );
    if ( player->direction.x < -0.9 && dashingDirection == 1 ) {
        dashTimer = 0;
        walkTimer = 0;
        dashingDirection = -1;
    } else if ( player->direction.x > 0.9 && dashingDirection == -1) {
        dashTimer = 0;
        walkTimer = 0;
        dashingDirection = 1;
    }
    dashTimer += dt;
    if ( dashTimer > player->dashLength ) {
        if ( walkTimer > player->walkLength ) {
            player->switchState( new WalkingState(player) );
        } else {
            player->switchState( new RunningState(player, dashingDirection) );
        }
    }
    if ( !player->onGround ) {
        player->switchState( new AirborneState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->releasedJump ) {
            player->switchState( new JumpSquatState(player) );
        }
    } else {
        player->releasedJump = true;
    }
}
JumpSquatState::JumpSquatState( Player* player) {
    this->player = player;
    jumpSquatTimer = 0;
}
JumpSquatState::~JumpSquatState() {
}
PlayerState JumpSquatState::getType() {
    return PlayerState::JumpSquat;
}
void JumpSquatState::update( Player* player, double dt ) {
    player->sprite->play( player->jumpSquatAnimation );
    player->sprite->setFrameTime(sf::seconds(player->jumpSquatLength/(float)player->jumpSquatAnimation.getSize()));
    player->fastFalling = false;
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    if ( player->jumpButton ) {
        player->releasedJump = false;
        jumpSquatTimer += dt;
    } else {
        player->releasedJump = true;
        glm::vec2 newvel;
        newvel.x = vel.x;
        newvel.y = vel.y-player->shortHopHeight;
        player->myBody->SetLinearVelocity( toB2(newvel) );
        player->switchState( new JumpingState(player) );
        return;
    }
    if ( jumpSquatTimer > player->jumpSquatLength ) {
        glm::vec2 newvel;
        newvel.x = vel.x;
        newvel.y = vel.y-player->fullHopHeight;
        player->myBody->SetLinearVelocity( toB2(newvel) );
        player->switchState( new JumpingState(player) );
    }
}

JumpingState::JumpingState( Player* player) {
    this->player = player;
    walkTimer = 0;
    canWallJump = false;
    player->fastFalling = false;
    player->sprite->play( player->airborneAnimation );
    player->sprite->setFrameTime(sf::seconds(0.15));
}
JumpingState::~JumpingState() {
}
PlayerState JumpingState::getType() {
    return PlayerState::Jumping;
}
void JumpingState::update( Player* player, double dt ) {
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    if ( vel.y < 0 ) {
        // Resist gravity a tad if they are still holding the jump button.
        if ( player->jumpButton ) {
            player->myBody->ApplyForceToCenter( b2Vec2(0,-player->jumpHelpAmount*dt), true );
        }
    } else {
        player->switchState( new AirborneState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->canDoubleJump && player->releasedJump ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->direction.x*player->doubleJumpHeight*.5,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
            player->canDoubleJump = false;
            player->releasedJump = false;
        }
    } else {
        player->releasedJump = true;
    }
    // aircontrol
    vel = toGLM(player->myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(player->direction.x*player->playerSpeed*player->airControlMultiplier);
    newvel.y = vel.y;
    player->myBody->SetLinearVelocity( toB2(newvel) );

    // Walljumping
    if ( player->direction.x != 0 ) {
        walkTimer += dt;
    } else {
        canWallJump = true;
        walkTimer = 0;
    }
    if ( walkTimer > player->walkLength && canWallJump ) {
        if ( player->direction.x > 0.9 && player->canWallJumpRight ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->doubleJumpHeight,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
        } else if ( player->direction.x < -0.9 && player->canWallJumpLeft ) {
            player->myBody->SetLinearVelocity( b2Vec2(-player->doubleJumpHeight,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
        }
        canWallJump = false;
    }
    if ( player->airDodgePressed ) {
        player->switchState( new AirDodgeState(player, player->direction ) );
    }
}
AirborneState::AirborneState( Player* player) {
    this->player = player;
    player->sprite->play( player->airborneAnimation );
    player->sprite->setFrameTime(sf::seconds(0.15));
    walkTimer = 0;
    canWallJump = false;
}
AirborneState::~AirborneState() {
    player->fastFalling = false;
}
PlayerState AirborneState::getType() {
    return PlayerState::Airborne;
}
void AirborneState::update( Player* player, double dt ) {
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(player->direction.x*player->playerSpeed*player->airControlMultiplier);
    newvel.y = vel.y;
    if ( player->direction.y > 0.9 && !player->fastFalling ) {
        newvel.y += player->fastFallSpeed;
        player->fastFalling = true;
    }
    player->myBody->SetLinearVelocity( toB2(newvel) );
    if ( player->onGround ) {
        player->switchState( new WalkingState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->canDoubleJump && player->releasedJump ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->direction.x*player->doubleJumpHeight*.5,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
            player->canDoubleJump = false;
            player->releasedJump = false;
        }
    } else {
        player->releasedJump = true;
    }
    // Walljumping
    if ( player->direction.x != 0 ) {
        walkTimer += dt;
    } else {
        walkTimer = 0;
        canWallJump = true;
    }
    if ( walkTimer > player->walkLength && canWallJump ) {
        canWallJump = false;
        if ( player->direction.x > 0.9 && player->canWallJumpRight ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->doubleJumpHeight,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
        } else if ( player->direction.x < -0.9 && player->canWallJumpLeft ) {
            player->myBody->SetLinearVelocity( b2Vec2(-player->doubleJumpHeight,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
        }
    }
    if ( player->airDodgePressed ) {
        player->switchState( new AirDodgeState(player, player->direction ) );
    }
}

RunningState::RunningState( Player* player, float direction ) {
    dashingDirection = direction;
    this->player = player;
    player->sprite->play( player->runningAnimation );
    player->sprite->setFrameTime(sf::seconds(player->dashLength/(float)player->dashingAnimation.getSize()));
}

RunningState::~RunningState() {
}

PlayerState RunningState::getType() {
    return PlayerState::Running;
}

void RunningState::update( Player* player, double dt ) {
    glm::vec2 vel = dashingDirection*player->playerSpeed*player->dashingMultiplier*glm::rotate( player->groundHitNormal, (float)M_PI/2.f );
    player->myBody->SetLinearVelocity( toB2(vel) );
    if ( fabs( player->direction.x ) < 0.7 ) {
        player->switchState( new WalkingState(player) );
    }
    if ( !player->onGround ) {
        player->switchState( new AirborneState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->releasedJump ) {
            player->switchState( new JumpSquatState(player) );
        }
    } else {
        player->releasedJump = true;
    }
}

AirDodgeState::AirDodgeState( Player* player, glm::vec2 direction ) {
    airDirection = direction;
    this->player = player;
}

AirDodgeState::~AirDodgeState() {
}

PlayerState AirDodgeState::getType() {
    return PlayerState::AirDodge;
}

void AirDodgeState::update( Player* player, double dt ) {
}
