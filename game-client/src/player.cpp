#include "player.h"

Player::Player( std::string resource, glm::vec2 pos, sf::View& view ) {
    hitLength = 0.2;
    successfulTech = false;
    directionalInfluence = 1.f;
    techLength = 20.f/60.f;
    frickedUpLength = 50.f/60.f;
    jumpHelpAmount = 2.f;
    damageBoostLength = 0.8;
    damageBoostTimer = 0;
    shockLength = 0.4;
    deadZone = 0.25; // in percentage
    walkLength = 0.06; // Time in seconds to wait for stick to smash, before walking
    jumpSquatLength = 0.08; // Time in seconds to wait for button release for a short hop.
    dashLength = 0.30; // in seconds
    playerWidth = .35; // in meters
    playerHeight = .35; // in meters
    airDodgeVelocity = 18;
    airDodgeTime = 0.3;
    turnAroundTime = 0.4;
    fastFallSpeed = 10;
    landLength = 0.2;
    fastFalling = false;
    playerSpeed = 5; // in meters per second
    dashingMultiplier = 2; // in percentage
    doubleJumpHeight = 12; // in meters per second
    fullHopHeight = 14; // actually a force, in newtons
    shortHopHeight = 8; // in newtons
    canWallJumpLeft = false;
    canWallJumpRight = false;
    canDoubleJump = true;
    releasedJump = true;
    airDodgePressed = false;
    airControlMultiplier = 4;
    flashTimer = 0;
    flashLength = 0;
    techTimer = 0;
    frickedUpTimer = 0;
    shakeLength = 0;
    shakeStrength = 1;
    newState = nullptr;
    this->view = &view;
    direction = glm::vec2(0,0);
    setUpSprite( resource );
    setUpBody();
    setUpSounds();
    myBody->SetTransform(toB2(pos),0);
    smoothCamera = pos*64.f;
    controllerID = 0;
    currentState = new IdleState(this);
    onGround = false;
    touchingCeiling = false;
    touchingWallLeft = false;
    touchingWallRight = false;
}

void Player::setUpSprite( std::string resource ) {
    texture = Resources->getTexture(resource);
    // Walking animation
    airborneAnimation.setSpriteSheet(*texture);
    airborneAnimation.addFrame(sf::IntRect(1,1,48,48));
	airborneAnimation.addFrame(sf::IntRect(51,1,48,48));

    airDodgeAnimation.setSpriteSheet(*texture);
	airDodgeAnimation.addFrame(sf::IntRect(101,1,48,48));
	airDodgeAnimation.addFrame(sf::IntRect(151,1,48,48));
	airDodgeAnimation.addFrame(sf::IntRect(201,1,48,48));

    fallingAnimation.setSpriteSheet(*texture);
	fallingAnimation.addFrame(sf::IntRect(251,1,48,48));
	fallingAnimation.addFrame(sf::IntRect(301,1,48,48));

    idleAnimation.setSpriteSheet(*texture);
	idleAnimation.addFrame(sf::IntRect(351,1,48,48));
	idleAnimation.addFrame(sf::IntRect(401,1,48,48));
	idleAnimation.addFrame(sf::IntRect(451,1,48,48));
	idleAnimation.addFrame(sf::IntRect(1,51,48,48));
	idleAnimation.addFrame(sf::IntRect(51,51,48,48));
	idleAnimation.addFrame(sf::IntRect(101,51,48,48));
	idleAnimation.addFrame(sf::IntRect(151,51,48,48));
	idleAnimation.addFrame(sf::IntRect(201,51,48,48));
	idleAnimation.addFrame(sf::IntRect(251,51,48,48));
	idleAnimation.addFrame(sf::IntRect(301,51,48,48));

    jumpingAnimation.setSpriteSheet(*texture);
	jumpingAnimation.addFrame(sf::IntRect(351,51,48,48));
	jumpingAnimation.addFrame(sf::IntRect(401,51,48,48));
	jumpingAnimation.addFrame(sf::IntRect(451,51,48,48));

    jumpSquatAnimation.setSpriteSheet(*texture);
	jumpSquatAnimation.addFrame(sf::IntRect(1,101,48,48));
	jumpSquatAnimation.addFrame(sf::IntRect(51,101,48,48));
	jumpSquatAnimation.addFrame(sf::IntRect(101,101,48,48));

    landingAnimation.setSpriteSheet(*texture);
	landingAnimation.addFrame(sf::IntRect(101,101,48,48));
	landingAnimation.addFrame(sf::IntRect(51,101,48,48));
	landingAnimation.addFrame(sf::IntRect(1,101,48,48));

    knockBackAnimation.setSpriteSheet(*texture);
	knockBackAnimation.addFrame(sf::IntRect(151,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(201,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(251,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(301,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(351,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(401,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(451,101,48,48));
	knockBackAnimation.addFrame(sf::IntRect(1,151,48,48));
	knockBackAnimation.addFrame(sf::IntRect(51,151,47,48));
	knockBackAnimation.addFrame(sf::IntRect(100,151,48,48));
	knockBackAnimation.addFrame(sf::IntRect(150,151,48,48));
	knockBackAnimation.addFrame(sf::IntRect(200,151,48,48));
	//knockBackAnimation.addFrame(sf::IntRect(250,151,48,48));
	//knockBackAnimation.addFrame(sf::IntRect(300,151,48,48));
	//knockBackAnimation.addFrame(sf::IntRect(350,151,48,48));

    knockBackRecoverAnimation.setSpriteSheet(*texture);
	knockBackRecoverAnimation.addFrame(sf::IntRect(400,151,48,48));
	knockBackRecoverAnimation.addFrame(sf::IntRect(450,151,48,48));
	knockBackRecoverAnimation.addFrame(sf::IntRect(1,201,48,48));
	knockBackRecoverAnimation.addFrame(sf::IntRect(51,201,48,48));

    loseAnimation.setSpriteSheet(*texture);
	loseAnimation.addFrame(sf::IntRect(101,201,48,48));
	loseAnimation.addFrame(sf::IntRect(151,201,48,48));
	loseAnimation.addFrame(sf::IntRect(201,201,48,48));
	loseAnimation.addFrame(sf::IntRect(251,201,48,48));
	loseAnimation.addFrame(sf::IntRect(301,201,48,48));
	loseAnimation.addFrame(sf::IntRect(351,201,48,48));
	loseAnimation.addFrame(sf::IntRect(401,201,48,48));
	loseAnimation.addFrame(sf::IntRect(451,201,48,48));
	loseAnimation.addFrame(sf::IntRect(1,251,48,48));
	loseAnimation.addFrame(sf::IntRect(51,251,48,48));

    dashAnimation.setSpriteSheet(*texture);
	dashAnimation.addFrame(sf::IntRect(101,251,48,48));
	dashAnimation.addFrame(sf::IntRect(151,251,48,48));
	dashAnimation.addFrame(sf::IntRect(201,251,48,48));
	dashAnimation.addFrame(sf::IntRect(251,251,48,48));

    runAnimation.setSpriteSheet(*texture);
	runAnimation.addFrame(sf::IntRect(301,251,48,48));
	runAnimation.addFrame(sf::IntRect(351,251,48,48));
	runAnimation.addFrame(sf::IntRect(401,251,48,48));
	runAnimation.addFrame(sf::IntRect(451,251,48,48));
	runAnimation.addFrame(sf::IntRect(101,251,48,48));
	runAnimation.addFrame(sf::IntRect(151,251,48,48));
	runAnimation.addFrame(sf::IntRect(201,251,48,48));
	runAnimation.addFrame(sf::IntRect(251,251,48,48));

    shockedAnimation.setSpriteSheet(*texture);
	shockedAnimation.addFrame(sf::IntRect(1,301,48,48));
	shockedAnimation.addFrame(sf::IntRect(51,301,48,48));

    tecAnimation.setSpriteSheet(*texture);
	tecAnimation.addFrame(sf::IntRect(101,301,48,48));
	tecAnimation.addFrame(sf::IntRect(151,301,48,48));
	tecAnimation.addFrame(sf::IntRect(201,301,48,48));
	tecAnimation.addFrame(sf::IntRect(251,301,48,48));
	tecAnimation.addFrame(sf::IntRect(301,301,48,48));

    slideAnimation.setSpriteSheet(*texture);
	slideAnimation.addFrame(sf::IntRect(351,301,48,48));
	slideAnimation.addFrame(sf::IntRect(401,301,48,48));

    specialFallAnimation.setSpriteSheet(*texture);
	specialFallAnimation.addFrame(sf::IntRect(451,301,48,48));
	specialFallAnimation.addFrame(sf::IntRect(1,351,48,48));
	specialFallAnimation.addFrame(sf::IntRect(51,351,48,48));
	specialFallAnimation.addFrame(sf::IntRect(101,351,48,48));

    wallJumpAnimation.setSpriteSheet(*texture);
	wallJumpAnimation.addFrame(sf::IntRect(151,351,48,48));
	wallJumpAnimation.addFrame(sf::IntRect(201,351,48,48));
	wallJumpAnimation.addFrame(sf::IntRect(251,351,48,48));
	wallJumpAnimation.addFrame(sf::IntRect(301,351,48,48));
	wallJumpAnimation.addFrame(sf::IntRect(351,351,48,48));

    winAnimation.setSpriteSheet(*texture);
	winAnimation.addFrame(sf::IntRect(401,351,48,48));
	winAnimation.addFrame(sf::IntRect(451,351,48,48));
	winAnimation.addFrame(sf::IntRect(1,401,48,48));
	winAnimation.addFrame(sf::IntRect(51,401,48,48));
	winAnimation.addFrame(sf::IntRect(101,401,48,48));
	winAnimation.addFrame(sf::IntRect(151,401,48,48));
	winAnimation.addFrame(sf::IntRect(201,401,48,48));
	winAnimation.addFrame(sf::IntRect(251,401,48,48));
	winAnimation.addFrame(sf::IntRect(301,401,48,48));
	winAnimation.addFrame(sf::IntRect(351,401,48,48));
	winAnimation.addFrame(sf::IntRect(401,401,48,48));
	winAnimation.addFrame(sf::IntRect(451,401,48,48));
	winAnimation.addFrame(sf::IntRect(1,451,48,48));
	winAnimation.addFrame(sf::IntRect(51,451,48,48));
	winAnimation.addFrame(sf::IntRect(101,451,48,48));
	winAnimation.addFrame(sf::IntRect(151,451,48,48));
	winAnimation.addFrame(sf::IntRect(201,451,48,48));
	winAnimation.addFrame(sf::IntRect(251,451,48,48));
	winAnimation.addFrame(sf::IntRect(301,451,48,48));

    hitAnimation.setSpriteSheet(*texture);
	hitAnimation.addFrame(sf::IntRect(151,101,48,48));

    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play(idleAnimation);
    sprite->scale(2,2);
}

void Player::setUpSounds() {
    dashSound = sf::Sound(*Resources->getSound( "assets/audio/effects/dash.ogg" ));
    jump1Sound = sf::Sound(*Resources->getSound( "assets/audio/effects/jump.ogg" ));
    jump2Sound = sf::Sound(*Resources->getSound( "assets/audio/effects/jump4.ogg" ));
    techSound = sf::Sound(*Resources->getSound( "assets/audio/effects/jump5.ogg" ));
    hurtSound = sf::Sound(*Resources->getSound( "assets/audio/effects/hurt.ogg" ));
    wallJumpSound = sf::Sound(*Resources->getSound( "assets/audio/effects/jump3.ogg" ));
    airDodgeSound = sf::Sound(*Resources->getSound( "assets/audio/effects/jump2.ogg" ));
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
    boxShape.SetAsBox(playerWidth/2.f-0.03,playerHeight/2.f);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.friction = 2;
    boxFixtureDef.restitution = 0;
    myBody->CreateFixture(&boxFixtureDef);
    boxFixtureDef.shape = &circleShape;
    myBody->CreateFixture(&boxFixtureDef);
    circleShape.m_p.Set(0,-playerHeight/2.f);
    myBody->CreateFixture(&boxFixtureDef);
    flipped = false;
}

Player::~Player() {
    if ( currentState ) {
        delete currentState;
    }
    if ( newState ) {
        delete newState;
    }
    delete sprite;
    myBody->GetWorld()->DestroyBody(myBody);
}

void Player::flash(sf::Color c, float length, float period) {
    flashLength = length;
    flashPeriod = period;
    flashColor = c;
}

void Player::shake(float strength, float length, float period) {
    shakeLength = length;
    shakeStrength = strength;
    shakePeriod = period;
}

void Player::draw( sf::RenderWindow& window ) {
    sf::Text stateText;
    stateText.setFont( *Resources->getFont( "assets/fonts/arial.ttf" ) );
    stateText.setString( StateString[currentState->getType()] );
    stateText.setCharacterSize(20);
    stateText.setStyle(sf::Text::Bold);
    //stateText.setColor(sf::Color::Red);
    stateText.setPosition(myBody->GetWorldCenter().x*64-48, myBody->GetWorldCenter().y*64-48);
    window.draw( *sprite );
    window.draw( stateText );
}

void Player::update( double dt ) {
    detectGround();
    detectWalls();
    // Damage boosting
    if ( damageBoostTimer > 0 ) {
        damageBoostTimer -= dt;
    } else {
        damageBoostTimer = 0;
    }
    // Flashing
    if ( flashLength != -1 && flashLength > 0 ) {
        if ( fmod(flashLength,flashPeriod*2)<flashPeriod ) {
            sprite->setColor( flashColor );
        } else {
            sprite->setColor( sf::Color(255,255,255,255) );
        }
        flashLength -= dt;
    } else if ( flashLength != -1 ) {
        sprite->setColor( sf::Color(255,255,255,255) );
    } else {
        flashTimer += dt;
        if ( fmod(flashTimer,flashPeriod*2)<flashPeriod ) {
            sprite->setColor( flashColor );
        } else {
            sprite->setColor( sf::Color(255,255,255,255) );
        }
    }
    // Teching logic
    if ( airDodgePressed && !onGround && frickedUpTimer <= 0 && techTimer <= 0 ) {
        techTimer = techLength;
    }
    if ( techTimer <= 0 ) {
        successfulTech = false;
    }
    if ( (onGround || touchingWallLeft || touchingWallRight || touchingCeiling) && techTimer > 0 && frickedUpTimer <= 0 ) {
        techTimer = 0;
        successfulTech = true;
    }
    if ( (onGround || touchingWallLeft || touchingWallRight || touchingCeiling) && techTimer <= 0 && frickedUpTimer <= 0 ) {
        frickedUpTimer = frickedUpLength;
    }
    frickedUpTimer -= dt;
    if ( techTimer > 0 && techTimer-dt < 0 ) {
        frickedUpTimer = frickedUpLength;
    }
    techTimer -= dt;
    /*for (int i=0;i<sf::Joystick::getButtonCount(0);i++ ) {
        if (sf::Joystick::isButtonPressed(0,i)) {
            std::cout << i << std::endl;
        }
    }
    std::cout << "X: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::X) << "\n";
    std::cout << "Y: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::Y) << "\n";
    std::cout << "Z: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::Z) << "\n";
    std::cout << "R: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::R) << "\n";
    std::cout << "U: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::U) << "\n";
    std::cout << "V: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::V) << "\n";
    std::cout << "PovX: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::PovX) << "\n";
    std::cout << "PovX: " << sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::PovY) << "\n\n";*/

    bool controllerFound = false;
    // Scan for an Xbox controller...
    for ( int i=0;i<4;i++ ) {
        if ( sf::Joystick::isConnected(i) && sf::Joystick::getIdentification(i).vendorId == 1118 && sf::Joystick::getIdentification(i).productId == 654) {
            controllerID = i;
            controllerFound = true;
            break;
        }
    }
    direction = glm::vec2(0,0);
    if ( sf::Joystick::isConnected(controllerID) && controllerFound) {
        if ( sf::Joystick::hasAxis(controllerID,sf::Joystick::Axis::X) && sf::Joystick::hasAxis(controllerID,sf::Joystick::Axis::Y) ) {
            direction = glm::vec2( sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::Y) );
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
			direction += glm::vec2(0,-1);
		}
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) || sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) {
			direction += glm::vec2(0,1);
		}
		if ( !sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) ) {
            direction *= .8;
        }
    }
    if ( sf::Joystick::isConnected( controllerID ) && controllerFound ) {
        if ( sf::Joystick::isButtonPressed(controllerID,2) || sf::Joystick::isButtonPressed(controllerID,3) ) {
            jumpButton = true;
        } else {
            jumpButton = false;
        }
    } else {
        if (sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) ) {
            jumpButton = true;
        } else {
            jumpButton = false;
        }
    }
    if ( sf::Joystick::isConnected( controllerID ) && controllerFound && sf::Joystick::hasAxis(controllerID,sf::Joystick::Axis::Z) ) {
            float check = sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::Z);
            float check2 = sf::Joystick::getAxisPosition(controllerID,sf::Joystick::Axis::R);
#ifdef WIN32
            if( check > 90 || check < -90 ) {
#else
            if( check > 90 || check2 > 90 ) {
#endif
                airDodgePressed = true;
            } else {
                airDodgePressed = false;
            }
    } else {
        if (sf::Keyboard::isKeyPressed( sf::Keyboard::R ) ) {
            airDodgePressed = true;
        } else {
            airDodgePressed = false;
        }
    }

    // 25% deadzone
    if ( glm::length(direction) < deadZone ) {
        direction = glm::vec2(0,0);
    }

    if ( newState ) {
        delete currentState;
        currentState = newState;
        currentState->init();
        newState = nullptr;
    }
    currentState->update(this, dt);
    if (onGround ) {
        canDoubleJump = true;
    }

    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    if ( vel.x > 0.01 && flipped) {
        sprite->scale(-1,1);
        flipped = false;
    } else if ( vel.x < -0.01 && !flipped ) {
        sprite->scale(-1,1);
        flipped = true;
    }

    glm::vec2 shakeAmount(0,0);
    glm::vec2 shakeAmount2(0,0);

    if ( shakeLength > 0 && fmod(shakeLength,shakePeriod*2) < shakePeriod ) {
        shakeAmount = glm::vec2(Random->f(-1.f, 1.f),Random->f(-1.f, 1.f) );
        shakeAmount *= shakeStrength;
        shakeAmount2 = glm::vec2(Random->f(-1.f, 1.f),Random->f(-1.f, 1.f) );
        shakeAmount2 *= shakeStrength/2.f;
    } 
    if (shakeLength > 0 ) {
        shakeLength -= dt;
    }

    lastDirection = direction;
    b2Vec2 pos = myBody->GetWorldCenter();
    float ang = myBody->GetAngle();
    // We'll assume 64 pixels is a meter
    if ( !flipped ) {
        sprite->setPosition( pos.x*64-48+shakeAmount.x, pos.y*64-74+shakeAmount.y );
    } else {
        sprite->setPosition( pos.x*64+48+shakeAmount.x, pos.y*64-74+shakeAmount.y );
    }
    position = glm::vec2((float)pos.x*64.f,(float)pos.y*64.f);
    sprite->setRotation( ang*180/3.149562 );
    vel = toGLM(myBody->GetLinearVelocity())*10.f;
    smoothCamera += (glm::vec2( (float)pos.x*64, (float)pos.y*64 )+vel-smoothCamera)*(float)dt*4.f;
    view->setCenter( toSFML(smoothCamera+shakeAmount2) );
    sprite->update( sf::seconds(dt) );
}

void Player::detectGround() {
    b2AABB testAABB;
    b2Vec2 pos = myBody->GetWorldCenter();
    testAABB.lowerBound = b2Vec2(pos.x-0.001, pos.y);
    testAABB.upperBound = b2Vec2(pos.x+0.001, pos.y+playerHeight+0.05);
    MapQueryCallback queryCallback;
    physicalWorld->get().QueryAABB( &queryCallback, testAABB );
    onGround = queryCallback.foundMap;

    testAABB.lowerBound = b2Vec2(pos.x-0.001, pos.y-playerHeight-0.05);
    testAABB.upperBound = b2Vec2(pos.x+0.001, pos.y);
    physicalWorld->get().QueryAABB( &queryCallback, testAABB );
    touchingCeiling = queryCallback.foundMap;

    b2AABB testAABB2;
    testAABB2.lowerBound = b2Vec2(pos.x-1, pos.y-playerHeight-1);
    testAABB2.upperBound = b2Vec2(pos.x+1, pos.y+playerHeight+1);
    MapQueryCallback queryCallback2;
    physicalWorld->get().QueryAABB( &queryCallback2, testAABB2 );

    //groundHitNormal = glm::vec2(0,0);
    if (queryCallback2.foundMap) {
        b2RayCastInput input;
        input.p1 = pos;
        input.p2 = pos+b2Vec2(0,playerHeight+10);
        input.maxFraction = 1;
        b2RayCastOutput output;
        float closestFrac = input.maxFraction+0.1;
        glm::vec2 currentOutput = glm::vec2(0.f,-1.f);
        for ( b2Fixture* f : queryCallback2.hitFixtures ) {
            if( f->RayCast(&output,input,0) && toGLM(output.normal) != glm::vec2(0,0) && output.fraction < closestFrac ) {
                closestFrac = output.fraction;
                currentOutput = toGLM(output.normal);
            }
        }
        groundHitNormal = currentOutput;

        if ( direction.x > 0 ) {
            input.p1 = pos;
            input.p2 = pos+b2Vec2(0,playerHeight+10);
            input.p1 += b2Vec2( .5, 0);
            input.p2 += b2Vec2( .5, 0);
            input.maxFraction = 1;
            closestFrac = input.maxFraction+0.1;
            currentOutput = glm::vec2(0.f,-1.f);
            for ( b2Fixture* f : queryCallback2.hitFixtures ) {
                if( f->RayCast(&output,input,0) && toGLM(output.normal) != glm::vec2(0,0) && output.fraction < closestFrac ) {
                    closestFrac = output.fraction;
                    currentOutput = toGLM(output.normal);
                }
            }
            if ( closestFrac != input.maxFraction+0.1 ) {
                // on level ground approaching upward slope to the right
                if ( currentOutput.x < 0 && groundHitNormal.y == -1 ) {
                    groundHitNormal = glm::normalize(groundHitNormal + currentOutput);
                // on upward slope approaching level ground to the right
                } else if ( groundHitNormal.x < 0 && currentOutput.y == -1 ) {
                    groundHitNormal = glm::normalize(groundHitNormal + currentOutput);
                // on level ground approaching downward slope to the right
                } else if ( groundHitNormal.y == -1 && currentOutput.x > 0 ) {
                // on downward slope approaching level ground to the right
                } else if ( groundHitNormal.x > 0 && currentOutput.y == -1 ) {
                }
            }
        } else {
            input.p1 = pos;
            input.p2 = pos+b2Vec2(0,playerHeight+10);
            input.p1 -= b2Vec2( .5, 0);
            input.p2 -= b2Vec2( .5, 0);
            input.maxFraction = 1;
            closestFrac = input.maxFraction+0.1;
            currentOutput = glm::vec2(0.f,-1.f);
            for ( b2Fixture* f : queryCallback2.hitFixtures ) {
                if( f->RayCast(&output,input,0) && toGLM(output.normal) != glm::vec2(0,0) && output.fraction < closestFrac ) {
                    closestFrac = output.fraction;
                    currentOutput = toGLM(output.normal);
                }
            }
            if ( closestFrac != input.maxFraction+0.1 ) {
                // on level ground approaching upward slope to the left
                if ( currentOutput.x > 0 && groundHitNormal.y == -1 ) {
                    groundHitNormal = glm::normalize(groundHitNormal + currentOutput);
                // on upward slope approaching level ground to the left
                } else if ( groundHitNormal.x > 0 && currentOutput.y == -1 ) {
                    groundHitNormal = glm::normalize(groundHitNormal + currentOutput);
                // on level ground approaching downward slope to the left
                } else if ( groundHitNormal.y == -1 && currentOutput.x < 0 ) {
                // on downward slope approaching level ground to the left
                } else if ( groundHitNormal.x < 0 && currentOutput.y == -1 ) {
                }
            }
        }
    }
    if ( groundHitNormal.x > 0.9 ) {
        groundHitNormal = glm::normalize(glm::vec2(1.f, -1.f));
    } else if (groundHitNormal.x < -0.9 ) {
        groundHitNormal = glm::normalize(glm::vec2(-1.f, -1.f));
    }

    glm::vec2 up = glm::vec2(0.f,-1.f);
    groundAngle = -acos(glm::dot(up,groundHitNormal))*180.f/M_PI;
}

void Player::detectWalls() {
    b2Vec2 pos = myBody->GetWorldCenter();

    b2AABB testAABB;
    testAABB.lowerBound = b2Vec2(pos.x, pos.y-playerHeight/2.f);
    testAABB.upperBound = b2Vec2(pos.x+playerWidth/2.f+0.3, pos.y+playerHeight/2.f);
    MapQueryCallback queryCallback;
    physicalWorld->get().QueryAABB( &queryCallback, testAABB );
    canWallJumpLeft = queryCallback.foundMap;

    testAABB.lowerBound = b2Vec2(pos.x, pos.y-playerHeight/3.f);
    testAABB.upperBound = b2Vec2(pos.x+playerWidth/2.f, pos.y+playerHeight/3.f);
    MapQueryCallback queryCallback2;
    physicalWorld->get().QueryAABB( &queryCallback2, testAABB );
    touchingWallRight = queryCallback2.foundMap;

    testAABB.lowerBound = b2Vec2(pos.x-playerWidth/2.f-0.3, pos.y-playerHeight/2.f);
    testAABB.upperBound = b2Vec2(pos.x, pos.y+playerHeight/2.f);
    MapQueryCallback queryCallback3;
    physicalWorld->get().QueryAABB( &queryCallback3, testAABB );
    canWallJumpRight = queryCallback3.foundMap;

    testAABB.lowerBound = b2Vec2(pos.x-playerWidth/2.f, pos.y-playerHeight/3.f);
    testAABB.upperBound = b2Vec2(pos.x, pos.y+playerHeight/3.f);
    MapQueryCallback queryCallback4;
    physicalWorld->get().QueryAABB( &queryCallback4, testAABB );
    touchingWallLeft = queryCallback4.foundMap;
}

void Player::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal ) {
}

Entity::Type Player::getType(){
    return Entity::Type::Player;
}

void Player::switchState( GenericPlayerState* state ) {
    if ( newState ) {
        delete newState;
    }
    newState = state;
}

void Player::damageBoost() {
    if ( damageBoostTimer == 0 ) {
        damageBoostTimer = damageBoostLength;
    }
}

bool Player::isDamageBoosted() {
    return damageBoostTimer != 0;
}
