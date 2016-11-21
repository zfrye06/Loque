#include "player.h"

Player::Player( std::string resource, sf::View& view ) {
    walkTimer = 0;
    jumpHelpAmount = 1;
    deadZone = 0.25; // in percentage
    walkLength = 0.06; // Time in seconds to wait for stick to smash, before walking
    jumpSquatLength = 0.06; // Time in seconds to wait for button release for a short hop.
    dashLength = 0.3; // in seconds
    playerWidth = .7; // in meters
    playerHeight = .6; // in meters
    playerSpeed = 5; // in meters per second
    dashingMultiplier = 2; // in percentage
    doubleJumpHeight = 9; // in meters per second
    fullHopHeight = 350; // actually a force, in newtons
    shortHopHeight = 280; // in newtons
    canDoubleJump = true;
    releasedJump = true;
    airControlMultiplier = 5;
    currentState = Player::State::Idle;
    this->view = &view;
    setUpSprite( resource );
    setUpBody();
}

void Player::setUpSprite( std::string resource ) {
    texture = Resources->getTexture(resource);
    // Walking animation
    currentAnimation.setSpriteSheet(*texture);
    currentAnimation.addFrame(sf::IntRect(69, 193, 68, 93));
    currentAnimation.addFrame(sf::IntRect(0, 0, 70, 96));
    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play( currentAnimation );
}

void Player::setUpBody() {
    // Physics....
    b2BodyDef myBodyDef;
    myBodyDef.fixedRotation = true;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0,0);
    myBodyDef.angle = 0;
    myBody = physicalWorld->get().CreateBody( &myBodyDef );
    myBody->SetUserData(this);
    b2PolygonShape boxShape;
    b2CircleShape circleShape;
    circleShape.m_p.Set(0,playerHeight/2.f);
    circleShape.m_radius = playerWidth/2.f;
    // Box is smaller than spheres.
    boxShape.SetAsBox(playerWidth/2.f-0.3,playerHeight/2.f);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.restitution = 0;
    myBody->CreateFixture(&boxFixtureDef);
    boxFixtureDef.shape = &circleShape;
    myBody->CreateFixture(&boxFixtureDef);
    circleShape.m_p.Set(0,-playerHeight/2.f);
    myBody->CreateFixture(&boxFixtureDef);
}

Player::~Player() {
    delete sprite;
}

void Player::draw( sf::RenderWindow& window ) {
    sf::Text stateText;
    stateText.setFont( *Resources->getFont( "assets/fonts/arial.ttf" ) );
    stateText.setString( StateString[currentState] );
    stateText.setCharacterSize(20);
    stateText.setStyle(sf::Text::Bold);
    stateText.setFillColor(sf::Color::Red);
    stateText.setPosition(myBody->GetWorldCenter().x*64, myBody->GetWorldCenter().y*64);
    window.draw( *sprite );
    window.draw( stateText );
}

void Player::update( double dt ) {
    detectGround();

    glm::vec2 direction(0,0);
    /*for (int i=0;i<11;i++ ) {
        if (sf::Joystick::isButtonPressed(0,i)) {
            std::cout << i << std::endl;
        }
    }*/
    if ( sf::Joystick::isConnected(0) && sf::Joystick::getButtonCount( 0 ) == 11) {
        if ( sf::Joystick::hasAxis(0,sf::Joystick::Axis::X) && sf::Joystick::hasAxis(0,sf::Joystick::Axis::Y) ) {
            direction = glm::vec2( sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::Y) );
            direction /= 100.f;
        }
    } else {
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right) || sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) {
			direction += glm::vec2(1,0);
		}
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) || sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) {
			direction += glm::vec2(-1,0);
		}
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) || sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ) {
			direction += glm::vec2(0,1);
		}
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) || sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) {
			direction += glm::vec2(0,-1);
		}
		if ( !sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) ) {
            direction *= .8;
        }
    }
    // 25% deadzone
    if ( glm::length(direction) < deadZone ) {
        direction = glm::vec2(0,0);
    }

    switch( currentState ) {
        case Player::State::Idle: {
                                      playerIdle(direction, dt);
                                      break;
                                  }
        case Player::State::Walking: {
                                      playerWalking(direction, dt);
                                      break;
                                  }
        case Player::State::Dashing: {
                                         playerDashing(direction, dt);
                                         break;
                                     }
        case Player::State::Running: {
                                         playerRunning(direction, dt);
                                         break;
                                     }
        case Player::State::Airborne: {
                                         playerAirborne(direction, dt);
                                         break;
                                     }
        case Player::State::JumpSquat: {
                                         playerJumpSquat(direction, dt);
                                         break;
                                     }
        case Player::State::Jumping: {
                                         playerJumping(direction, dt);
                                         break;
                                     }
        default: {
                     break;
                 }
    }
    if (onGround ) {
        canDoubleJump = true;
    }

    lastDirection = direction;
    b2Vec2 pos = myBody->GetWorldCenter();
    float ang = myBody->GetAngle();
    // We'll assume 64 pixels is a meter
    sprite->setPosition( pos.x*64-35, pos.y*64-50 );
    sprite->setRotation( ang*180/3.149562 );
    view->setCenter( pos.x*64, pos.y*64 );
    sprite->update( sf::seconds(dt) );
}

void Player::playerIdle( glm::vec2 direction, float dt ) {
    if ( direction.x != 0 ) {
        walkTimer += dt;
    } else {
        walkTimer = 0;
    }
    if ( walkTimer > walkLength ) {
        if ( fabs(direction.x) > 0.9 ) {
            currentState = Player::State::Dashing;
            if ( direction.x > 0 ) {
                dashTimer = 0;
                dashingDirection = 1;
            } else {
                dashTimer = 0;
                dashingDirection = -1;
            }
        } else {
            currentState = Player::State::Walking;
        }
    }
    if ( !onGround ) {
        currentState = Player::State::Airborne;
    }
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        jumpSquatTimer = 0;
        currentState = Player::State::JumpSquat;
    }
}

void Player::playerWalking( glm::vec2 direction, float dt) {
    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    glm::vec2 newvel = direction*playerSpeed;
    newvel.y = vel.y;
    myBody->SetLinearVelocity( toB2(newvel) );

    if ( newvel.x == 0 ) {
        currentState = Player::State::Idle;
    }
    if ( !onGround ) {
        currentState = Player::State::Airborne;
    }
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        jumpSquatTimer = 0;
        currentState = Player::State::JumpSquat;
    }
}

void Player::playerDashing( glm::vec2 direction, float dt ) {
    if ( fabs(direction.x) < 0.9 ) {
        walkTimer += dt;
    } else {
        walkTimer = 0;
    }
    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = dashingDirection*playerSpeed*dashingMultiplier;
    newvel.y = vel.y;
    myBody->SetLinearVelocity( toB2(newvel) );
    if ( direction.x < -0.9 && dashingDirection == 1 ) {
        walkTimer = 0;
        dashTimer = 0;
        dashingDirection = -1;
    } else if ( direction.x > 0.9 && dashingDirection == -1) {
        walkTimer = 0;
        dashTimer = 0;
        dashingDirection = 1;
    }
    dashTimer += dt;
    if ( dashTimer > dashLength ) {
        if ( walkTimer > walkLength ) {
            currentState = Player::State::Walking;
        } else {
            currentState = Player::State::Running;
        }
    }
    if ( !onGround ) {
        currentState = Player::State::Airborne;
    }
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        jumpSquatTimer = 0;
        currentState = Player::State::JumpSquat;
    }
}

void Player::playerRunning( glm::vec2 direction, float dt ) {
    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = dashingDirection*playerSpeed*dashingMultiplier;
    newvel.y = vel.y;
    myBody->SetLinearVelocity( toB2(newvel) );
    if ( fabs( direction.x ) < 0.7 ) {
        currentState = Player::State::Walking;
    }
    if ( !onGround ) {
        currentState = Player::State::Airborne;
    }
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        jumpSquatTimer = 0;
        currentState = Player::State::JumpSquat;
    }
}

void Player::playerAirborne( glm::vec2 direction, float dt ) {
    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(direction.x*playerSpeed*airControlMultiplier)*dt;
    newvel.y = vel.y;
    myBody->SetLinearVelocity( toB2(newvel) );
    if ( onGround ) {
        if ( direction.x < -0.9 ) {
            walkTimer = 0;
            dashTimer = 0;
            dashingDirection = -1;
            currentState = Player::State::Dashing;
        } else if ( direction.x > 0.9 ) {
            walkTimer = 0;
            dashTimer = 0;
            dashingDirection = 1;
            currentState = Player::State::Dashing;
        } else {
            currentState = Player::State::Walking;
        }
    }
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        if ( canDoubleJump && releasedJump ) {
            myBody->SetLinearVelocity( b2Vec2(direction.x*doubleJumpHeight,-doubleJumpHeight) );
            currentState = Player::State::Jumping;
            canDoubleJump = false;
            releasedJump = false;
        }
    } else {
        releasedJump = true;
    }
}

void Player::playerJumpSquat( glm::vec2 direction, float dt ) {
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        releasedJump = false;
        jumpSquatTimer += dt;
    } else {
        releasedJump = true;
        myBody->ApplyForceToCenter( b2Vec2(0,-shortHopHeight), true );
        currentState = Player::State::Jumping;
    }
    if ( jumpSquatTimer > jumpSquatLength ) {
        myBody->ApplyForceToCenter( b2Vec2(0,-fullHopHeight), true );
        currentState = Player::State::Jumping;
    }
}

void Player::playerJumping( glm::vec2 direction, float dt ) {
    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    if ( vel.y < 0 ) {
        // Resist gravity a tad if they are still holding the jump button.
        if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
            myBody->ApplyForceToCenter( b2Vec2(0,-jumpHelpAmount*dt), true );
        }
    } else {
        currentState = Player::State::Airborne;
    }
    if ( sf::Joystick::isButtonPressed(0,2) || sf::Joystick::isButtonPressed(0,3) || sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
        if ( canDoubleJump && releasedJump ) {
            myBody->ApplyForceToCenter( b2Vec2(direction.x*shortHopHeight,-fullHopHeight), true );
            currentState = Player::State::Jumping;
            canDoubleJump = false;
            releasedJump = false;
        }
    } else {
        releasedJump = true;
    }
    vel = toGLM(myBody->GetLinearVelocity());
    glm::vec2 newvel;
    newvel.x = vel.x+(direction.x*playerSpeed*airControlMultiplier)*dt;
    newvel.y = vel.y;
    myBody->SetLinearVelocity( toB2(newvel) );
}


void Player::detectGround() {
    b2AABB testAABB;
    b2Vec2 pos = myBody->GetWorldCenter();
    testAABB.lowerBound = b2Vec2(pos.x, pos.y);
    testAABB.upperBound = b2Vec2(pos.x+0.01, pos.y+playerHeight);
    MapQueryCallback queryCallback;
    physicalWorld->get().QueryAABB( &queryCallback, testAABB );
    onGround = queryCallback.foundMap;
}

void Player::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal ) {
}

Entity::Type Player::getType(){
    return Entity::Type::Player;
}

