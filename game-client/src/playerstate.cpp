#include "playerstate.h"

GenericPlayerState::GenericPlayerState( Player* player ) {
    this->player = player;
}
GenericPlayerState::GenericPlayerState() {
}
GenericPlayerState::~GenericPlayerState() {
}
void GenericPlayerState::init() {
}
PlayerState GenericPlayerState::getType() {
    return PlayerState::None;
}
void GenericPlayerState::update( Player* player, double dt ) {
}

WalkingState::WalkingState( Player* player ) {
    this->player = player;
}
void WalkingState::init() {
    player->sprite->play( player->runAnimation );
}
WalkingState::~WalkingState() {
}
PlayerState WalkingState::getType() {
    return PlayerState::Walking;
}
void WalkingState::update( Player* player, double dt ) {
    glm::vec2 vel = player->direction.x*player->playerSpeed*glm::rotate( player->groundHitNormal, (float)M_PI/2.f );
    player->myBody->SetLinearVelocity( toB2(vel) );
    player->sprite->setFrameTime(sf::seconds(.5/fabs(vel.x)));

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
}
void IdleState::init() {
    reset = false;
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
    if ( player->direction.x != 0 && reset ) {
        walkTimer += dt;
    } else if (player->direction.x == 0 ) {
        walkTimer = 0;
        reset = true;
    }
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    if ( fabs(vel.x) > 0.01 ) {
        player->sprite->play(player->slideAnimation);
    } else {
        player->sprite->play(player->idleAnimation);
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
}
void DashingState::init() {
    player->sprite->play( player->dashAnimation );
    player->sprite->setFrameTime(sf::seconds(player->dashLength/(float)player->dashAnimation.getSize()));
    player->dashSound.play();
    world->addEntity( new DashDust( player->position, dashingDirection == -1, player->groundAngle ) );
    world->bringToFront( player );
    dashTimer = 0;
    walkTimer = 0;
}
DashingState::~DashingState() {
}
PlayerState DashingState::getType() {
    return PlayerState::Dashing;
}
void DashingState::update( Player* player, double dt ) {
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
        world->addEntity( new DashDust( player->position, dashingDirection == -1, player->groundAngle ) );
        world->bringToFront( player );
        player->dashSound.play();
    } else if ( player->direction.x > 0.9 && dashingDirection == -1) {
        dashTimer = 0;
        walkTimer = 0;
        dashingDirection = 1;
        world->addEntity( new DashDust( player->position, dashingDirection == -1, player->groundAngle ) );
        world->bringToFront( player );
        player->dashSound.play();
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
}

void JumpSquatState::init(){
    player->sprite->play( player->jumpSquatAnimation );
    player->sprite->setFrameTime(sf::seconds(player->jumpSquatLength/(float)player->jumpSquatAnimation.getSize()));
    jumpSquatTimer = 0;
}
JumpSquatState::~JumpSquatState() {
}
PlayerState JumpSquatState::getType() {
    return PlayerState::JumpSquat;
}
void JumpSquatState::update( Player* player, double dt ) {
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

JumpingState::JumpingState( Player* player, float wallJumpDirection) {
    this->wallJumpDirection = wallJumpDirection;
    this->player = player;
}

void JumpingState::init() {
    walkTimer = 0;
    if ( fabs(player->direction.x) <= 0.3 ) {
        canWallJump = true;
    } else {
        canWallJump = false;
    }
    player->fastFalling = false;
    player->sprite->setLooped(false);
    if ( wallJumpDirection == 0 ) {
        if ( player->onGround ) {
            player->jump1Sound.play();
        } else {
            player->jump2Sound.play();
        }
        world->addEntity( new JumpDust( player->position, player->groundAngle, !player->onGround ) );
        world->bringToFront( player );
        player->sprite->play( player->jumpingAnimation );
        player->sprite->setFrameTime(sf::seconds(0.16));
    } else {
        glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
        world->addEntity( new WallJumpDust( player->position, vel.x < 0, player->groundAngle ) );
        world->bringToFront( player );
        player->wallJumpSound.play();
        player->sprite->play( player->wallJumpAnimation );
        player->sprite->setFrameTime(sf::seconds(0.1));
    }
    player->sprite->setFrame(0, true);
}
JumpingState::~JumpingState() {
    player->sprite->setLooped(true);
}
PlayerState JumpingState::getType() {
    return PlayerState::Jumping;
}
void JumpingState::update( Player* player, double dt ) {
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    if ( vel.y < 0 ) {
        // Resist gravity a tad if they are still holding the jump button.
        if ( player->jumpButton ) {
            player->myBody->ApplyForceToCenter( b2Vec2(0,-player->jumpHelpAmount), true );
        }
    } else {
        player->switchState( new AirborneState(player) );
    }
    // Double jumping
    if ( player->jumpButton ) {
        if ( player->canDoubleJump && player->releasedJump ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->direction.x*player->doubleJumpHeight/sqrt(2),-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player, 0) );
            player->canDoubleJump = false;
            player->releasedJump = false;
        }
    } else {
        player->releasedJump = true;
    }
    // Air control
    vel = toGLM(player->myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(player->direction.x*player->playerSpeed*player->airControlMultiplier*dt);
    if ( newvel.x > player->playerSpeed*player->dashingMultiplier ) {
        newvel.x = vel.x;
    } else if ( newvel.x < -player->playerSpeed*player->dashingMultiplier ) {
        newvel.x = vel.x;
    }
    newvel.y = vel.y;
    if ( player->touchingWallRight && newvel.x > 0 ) {
        newvel.x = vel.x;
    }
    if ( player->touchingWallLeft && newvel.x < 0 ) {
        newvel.x = vel.x;
    }
    player->myBody->SetLinearVelocity( toB2(newvel) );

    // Walljumping
    if ( canWallJump && fabs(player->direction.x) >= 0.9 ) {
        if ( player->direction.x > 0.9 && player->canWallJumpRight ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->doubleJumpHeight/sqrt(2),-player->doubleJumpHeight) );
            wallJumpDirection = 1;
            //player->flash(sf::Color(150,255,150,255),0.3,0.05);
            //player->switchState( new JumpingState(player,1) );
            init();
        } else if ( player->direction.x < -0.9 && player->canWallJumpLeft ) {
            wallJumpDirection = -1;
            player->myBody->SetLinearVelocity( b2Vec2(-player->doubleJumpHeight/sqrt(2),-player->doubleJumpHeight) );
            //player->flash(sf::Color(150,255,150,255),0.3,0.05);
            //layer->switchState( new JumpingState(player,-1) );
            init();
        }
        canWallJump = false;
    }
    if ( fabs(player->direction.x) <= 0.3 ) {
        canWallJump = true;
    }
    if ( player->airDodgePressed ) {
        player->switchState( new AirDodgeState(player, player->direction ) );
    }
}
AirborneState::AirborneState( Player* player) {
    this->player = player;
}

void AirborneState::init() {
    player->sprite->setLooped( false );
    player->sprite->play( player->airborneAnimation );
    player->sprite->setFrameTime(sf::seconds(0.15));
    walkTimer = 0;
    if ( fabs(player->direction.x) <= 0.3 ) {
        canWallJump = true;
    } else {
        canWallJump = false;
    }
}
AirborneState::~AirborneState() {
    player->sprite->setLooped( true );
    player->fastFalling = false;
}
PlayerState AirborneState::getType() {
    return PlayerState::Airborne;
}
void AirborneState::update( Player* player, double dt ) {
    if ( !player->sprite->isPlaying() ) {
        player->sprite->setLooped( true );
        player->sprite->play( player->fallingAnimation );
        player->sprite->setFrameTime(sf::seconds(0.15));
    }
    // Air control
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(player->direction.x*player->playerSpeed*player->airControlMultiplier*dt);
    if ( newvel.x > player->playerSpeed*player->dashingMultiplier ) {
        newvel.x = vel.x;
    } else if ( newvel.x < -player->playerSpeed*player->dashingMultiplier ) {
        newvel.x = vel.x;
    }
    newvel.y = vel.y;
    if ( player->touchingWallRight && newvel.x > 0 ) {
        newvel.x = vel.x;
    }
    if ( player->touchingWallLeft && newvel.x < 0 ) {
        newvel.x = vel.x;
    }
    if ( player->direction.y > 0.9 && !player->fastFalling ) {
        newvel.y += player->fastFallSpeed;
        player->fastFalling = true;
    }
    // Hit ground
    player->myBody->SetLinearVelocity( toB2(newvel) );
    if ( player->onGround ) {
        player->switchState( new LandingState(player) );
    }
    // Double jumping
    if ( player->jumpButton ) {
        if ( player->canDoubleJump && player->releasedJump ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->direction.x*player->doubleJumpHeight/sqrt(2),-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player) );
            player->canDoubleJump = false;
            player->releasedJump = false;
        }
    } else {
        player->releasedJump = true;
    }
    // Walljumping
    if ( canWallJump && fabs(player->direction.x) >= 0.9 ) {
        if ( player->direction.x > 0.9 && player->canWallJumpRight ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->doubleJumpHeight/sqrt(2),-player->doubleJumpHeight) );
            //player->flash(sf::Color(150,255,150,255),0.3,0.05);
            player->switchState( new JumpingState(player,1) );
        } else if ( player->direction.x < -0.9 && player->canWallJumpLeft ) {
            player->myBody->SetLinearVelocity( b2Vec2(-player->doubleJumpHeight/sqrt(2),-player->doubleJumpHeight) );
            //player->flash(sf::Color(150,255,150,255),0.3,0.05);
            player->switchState( new JumpingState(player,-1) );
        }
        canWallJump = false;
    }
    if ( fabs(player->direction.x) <= 0.3 ) {
        canWallJump = true;
    }
    if ( player->airDodgePressed ) {
        player->switchState( new AirDodgeState(player, player->direction ) );
    }
}

RunningState::RunningState( Player* player, float direction ) {
    dashingDirection = direction;
    this->player = player;
}

void RunningState::init() {
    walkTimer = 0;
    crossedNeutral = false;
    player->sprite->play( player->runAnimation );
    player->sprite->setFrameTime(sf::seconds(player->dashLength/(float)player->dashAnimation.getSize()));
}

RunningState::~RunningState() {
}

PlayerState RunningState::getType() {
    return PlayerState::Running;
}

void RunningState::update( Player* player, double dt ) {
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    if ( player->direction.x < -0.9 && dashingDirection == 1 && crossedNeutral ) {
        player->switchState( new TurningState(player, -1) );
    } else if ( player->direction.x > 0.9 && dashingDirection == -1 && crossedNeutral ) {
        player->switchState( new TurningState(player, 1) );
    }
    if ( (player->direction.x == 0 && player->direction.y == 0) || glm::length(player->direction) < 0.5 ) {
        crossedNeutral = true;
        walkTimer += dt;
    } else {
        if ( vel.x/fabs(vel.x) != dashingDirection ) {
            walkTimer += dt;
        } else {
            walkTimer = 0;
        }
    }
    if ( walkTimer >= player->walkLength ) {
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
    vel = player->direction.x*player->playerSpeed*player->dashingMultiplier*glm::rotate( player->groundHitNormal, (float)M_PI/2.f );
    player->myBody->SetLinearVelocity( toB2(vel) );
}

AirDodgeState::AirDodgeState( Player* player, glm::vec2 direction ) {
    airDirection = direction;
    this->player = player;
}

void AirDodgeState::init() {
    player->sprite->play( player->airDodgeAnimation );
    player->sprite->setFrameTime(sf::seconds(player->airDodgeTime/player->airDodgeAnimation.getSize()));
    vel = glm::normalize(airDirection)*player->airDodgeVelocity;
    if ( isnan(vel.x ) ) {
        vel.x = 0;
        vel.y = 0;
    }
    tweenX = tweeny::from(vel.x).to(0.f).during(player->airDodgeTime*1000).via(tweeny::easing::quadraticOut);
    tweenY = tweeny::from(vel.y).to(0.f).during(player->airDodgeTime*1000).via(tweeny::easing::quadraticOut);
    player->airDodgeSound.play();
}

AirDodgeState::~AirDodgeState() {
}

PlayerState AirDodgeState::getType() {
    return PlayerState::AirDodge;
}

void AirDodgeState::update( Player* player, double dt ) {
    glm::vec2 vel = glm::vec2(tweenX.step((int)(dt*1000)),tweenY.step((int)(dt*1000)));
    player->myBody->SetLinearVelocity( toB2(vel) );
    if ( tweenX.progress() >= 1.f && tweenY.progress() >= 1.f ) {
        player->switchState( new SpecialFallState(player) );
    }
    if ( player->onGround ) {
        player->switchState( new LandingState(player) );
    }
}


SpecialFallState::SpecialFallState( Player* player ) {
    this->player = player;
}

void SpecialFallState::init() {
    player->sprite->play( player->specialFallAnimation );
    player->sprite->setFrameTime(sf::seconds(0.15));
    player->flash(sf::Color(200,150,150,255),-1,0.08);
}

SpecialFallState::~SpecialFallState() {
    player->flash(sf::Color(255,255,255,255),0,0.1);
}

PlayerState SpecialFallState::getType() {
    return PlayerState::SpecialFall;
}

void SpecialFallState::update( Player* player, double dt ) {
    // Air control
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(player->direction.x*player->playerSpeed*player->airControlMultiplier*dt);
    if ( newvel.x > player->playerSpeed ) {
        newvel.x = vel.x;
    } else if ( newvel.x < -player->playerSpeed ) {
        newvel.x = vel.x;
    }
    newvel.y = vel.y;
    if ( player->touchingWallRight && newvel.x > 0 ) {
        newvel.x = vel.x;
    }
    if ( player->touchingWallLeft && newvel.x < 0 ) {
        newvel.x = vel.x;
    }
    if ( player->direction.y > 0.9 && !player->fastFalling ) {
        newvel.y += player->fastFallSpeed;
        player->fastFalling = true;
    }
    // Hit ground
    player->myBody->SetLinearVelocity( toB2(newvel) );
    if ( player->onGround ) {
        player->switchState( new LandingState(player) );
    }
}

TurningState::TurningState( Player* player, float direction ) {
    turnDirection = direction;
    this->player = player;
}

void TurningState::init() {
    player->sprite->play( player->slideAnimation );
    player->sprite->setFrameTime(sf::seconds(0.16));
    float d = turnDirection*player->playerSpeed*player->dashingMultiplier;
    tweenX = tweeny::from(-d).to(0).during(player->turnAroundTime*1000).via(tweeny::easing::quadraticOut);
    turnTimer = 0;
}

TurningState::~TurningState() {
}

PlayerState TurningState::getType() {
    return PlayerState::Turning;
}

void TurningState::update( Player* player, double dt ) {
    // If the code reaches here you should never play smash again probably
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    player->myBody->SetLinearVelocity( b2Vec2(tweenX.step((int)(dt*1000)),vel.y) );
    turnTimer += dt;
    if ( turnTimer > player->turnAroundTime ) {
        player->switchState( new DashingState(player,turnDirection) );
    }
}

LandingState::LandingState( Player* player ) {
    this->player = player;
}

void LandingState::init() {
    bufferedState = nullptr;
    landTimer = 0;
    reset = false;
    walkTimer = 0;
    world->addEntity( new LandingDust( player->position, player->groundAngle) );
    world->bringToFront( player );
    player->sprite->play( player->landingAnimation );
    player->sprite->setFrameTime(sf::seconds(player->landLength/(float)player->landingAnimation.getSize()));
}

LandingState::~LandingState() {
}

PlayerState LandingState::getType() {
    return PlayerState::Landing;
}

void LandingState::update( Player* player, double dt ) {
    landTimer += dt;
    if (landTimer > player->landLength ) {
        if ( bufferedState ) {
            player->switchState( bufferedState );
        } else {
            if ( fabs(player->direction.x) > 0.9 ) {
                player->switchState( new WalkingState(player) );
            } else {
                player->switchState( new IdleState(player) );
            }
        }
        return;
    }
    if ( player->direction.x != 0 && reset ) {
        walkTimer += dt;
    } else if (player->direction.x == 0 ) {
        walkTimer = 0;
        reset = true;
    }
    if ( walkTimer > player->walkLength ) {
        if ( fabs(player->direction.x) > 0.9 ) {
            if ( player->direction.x > 0 ) {
                if ( bufferedState ) {
                    delete bufferedState;
                    bufferedState = nullptr;
                }
                bufferedState =  new DashingState(player,1);
            } else {
                if ( bufferedState ) {
                    delete bufferedState;
                    bufferedState = nullptr;
                }
                bufferedState =  new DashingState(player,-1);
            }
        } else {
            if ( bufferedState ) {
                delete bufferedState;
                bufferedState = nullptr;
            }
            bufferedState =  new WalkingState(player);
        }
    }
    if ( !player->onGround ) {
        if ( bufferedState ) {
            delete bufferedState;
            bufferedState = nullptr;
        }
        player->switchState( new AirborneState(player) );
    }
    if ( player->jumpButton ) {
        if ( player->releasedJump ) {
            if ( bufferedState ) {
                delete bufferedState;
                bufferedState = nullptr;
            }
            bufferedState =  new JumpSquatState(player);
        }
    } else {
        player->releasedJump = true;
    }
}

ShockedState::ShockedState( Player* player, glm::vec2 impulse, int type, float duration ) {
    this->duration = duration;
    this->type = type;
    this->player = player;
    this->impulse = impulse;
}

void ShockedState::init() {
    player->damageBoost();
    shockTimer = 0;
    player->myBody->SetAwake( false );
    player->myBody->SetLinearVelocity( b2Vec2(0,0) );
    switch(type) {
        case 0: { player->sprite->play( player->shockedAnimation ); break; }
        case 1: { player->sprite->play( player->hitAnimation ); break; }
    }
    player->sprite->setFrameTime(sf::seconds(0.08));
}

ShockedState::~ShockedState() {
    player->myBody->SetAwake( true );
}

PlayerState ShockedState::getType() {
    return PlayerState::Shocked;
}

void ShockedState::update( Player* player, double dt ) {
    shockTimer += dt;
    player->myBody->SetAwake( false );
    player->myBody->SetLinearVelocity( b2Vec2(0,0) );
    if ( shockTimer > duration ) {
        player->switchState( new KnockbackState(player, impulse) );
    }
}

KnockbackState::KnockbackState( Player* player, glm::vec2 impulse ) {
    this->player = player;
    this->impulse = impulse;
}

void KnockbackState::init() {
    player->hurtSound.play();
    float strength = glm::length(impulse);
    glm::vec2 di = glm::normalize(player->direction*player->directionalInfluence*strength);
    glm::vec2 newImpulse = glm::normalize(impulse);
    // If it would make us face the wrong way entirely... Don't do any DI
    if ( !isnan(di.x) && glm::dot(glm::normalize(impulse),di) > 0 ) {
        newImpulse = glm::normalize(newImpulse+di);
    } else if ( !isnan(di.x) ) {
        // If we're pretty close to being the right way, snap the DI to the intended direction
        if ( glm::dot(glm::normalize(impulse),di) > -0.5 ) {
            if ( di.y > 0.8 ) {
                di.y = 1;
                di.x = 0;
            } else if ( di.y < -0.8 ) {
                di.y = -1;
                di.x = 0;
            } else if ( di.x < -0.8 ) {
                di.x = -1;
                di.y = 0;
            } else if ( di.x > 0.8 ) {
                di.x = 1;
                di.y = 0;
            }
            newImpulse = glm::normalize(newImpulse+di);
        }
    }
    player->myBody->SetLinearVelocity( toB2( newImpulse*strength ) );
    player->sprite->play( player->knockBackAnimation );
    player->sprite->setLooped( false );
    player->flash(sf::Color(200,150,150,255),(player->knockBackAnimation.getSize()+5)*0.07,0.09);
    player->sprite->setFrameTime(sf::seconds(0.07));
    player->sprite->setFrame(0,true);
}

KnockbackState::~KnockbackState() {
    player->sprite->setLooped( true );
}

PlayerState KnockbackState::getType() {
    return PlayerState::KnockBack;
}

void KnockbackState::update( Player* player, double dt ) {
    if ( !player->sprite->isPlaying() ) {
        if ( player->canDoubleJump && player->releasedJump && player->jumpButton && !player->onGround ) {
            player->myBody->SetLinearVelocity( b2Vec2(player->direction.x*player->doubleJumpHeight*.5,-player->doubleJumpHeight) );
            player->switchState( new JumpingState(player, 0) );
            player->canDoubleJump = false;
            player->releasedJump = false;
        }
        if ( (player->direction.y < -0.9 || fabs(player->direction.x) || player->airDodgePressed) && player->onGround ) {
            player->switchState( new KnockbackRecoverState(player) );
        }
    }
    if ( player->successfulTech ) {
        player->switchState( new KnockbackRecoverState(player,true) );
    }
    // Air control
    glm::vec2 vel = toGLM(player->myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(player->direction.x*player->playerSpeed*player->airControlMultiplier*dt);
    if ( newvel.x > player->playerSpeed ) {
        newvel.x = vel.x;
    } else if ( newvel.x < -player->playerSpeed ) {
        newvel.x = vel.x;
    }
    newvel.y = vel.y;
    if ( player->touchingWallRight && newvel.x > 0 ) {
        newvel.x = vel.x;
    }
    if ( player->touchingWallLeft && newvel.x < 0 ) {
        newvel.x = vel.x;
    }
    player->myBody->SetLinearVelocity( toB2( newvel ) );
}

KnockbackRecoverState::KnockbackRecoverState( Player* player, bool teched ) {
    this->teched = teched;
    this->player = player;
}

void KnockbackRecoverState::init() {
    world->addEntity( new LandingDust( player->position, player->groundAngle) );
    player->myBody->SetLinearVelocity( b2Vec2(0,0) );
    player->myBody->SetAwake(false);
    if ( teched ) {
        player->flash(sf::Color(120,255,120,255),0.2,0.1);
        player->sprite->play( player->tecAnimation );
    } else {
        player->sprite->scale(-1,1);
        player->flipped = !player->flipped;
        player->sprite->play( player->knockBackRecoverAnimation );
    }
    player->techSound.play();
    player->sprite->setLooped( false );
    player->sprite->setFrameTime(sf::seconds(0.1));
}

KnockbackRecoverState::~KnockbackRecoverState() {
    player->sprite->setLooped( true );
    player->myBody->SetAwake(true);
}

PlayerState KnockbackRecoverState::getType() {
    return PlayerState::KnockBackRecover;
}

void KnockbackRecoverState::update( Player* player, double dt ) {
    if ( !player->sprite->isPlaying() ) {
        player->switchState( new IdleState(player) );
    }
}

WinState::WinState( Player* player ) {
    this->player = player;
}

void WinState::init() {
    player->sprite->play( player->winAnimation );
    player->sprite->setLooped( false );
    player->sprite->setFrameTime(sf::seconds(0.1));
}

WinState::~WinState() {
    player->sprite->setLooped( true );
}

PlayerState WinState::getType() {
    return PlayerState::Win;
}

void WinState::update( Player* player, double dt ) {
    if ( !player->sprite->isPlaying() ) {
        player->sprite->setLooped( true );
        player->sprite->play( player->winRepeatAnimation );
        playerStats->sendStats();
    }
}
