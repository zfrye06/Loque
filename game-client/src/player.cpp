#include "player.h"

Player::Player( std::string resource, sf::View& view ) {
    jumpHelpAmount = 2;
    deadZone = 0.25; // in percentage
    walkLength = 0.06; // Time in seconds to wait for stick to smash, before walking
    jumpSquatLength = 0.08; // Time in seconds to wait for button release for a short hop.
    dashLength = 0.35; // in seconds
    playerWidth = .7; // in meters
    playerHeight = .6; // in meters
    fastFallSpeed = 10;
    fastFalling = false;
    playerSpeed = 5; // in meters per second
    dashingMultiplier = 2; // in percentage
    doubleJumpHeight = 12; // in meters per second
    fullHopHeight = 15; // actually a force, in newtons
    shortHopHeight = 8; // in newtons
    canWallJumpLeft = false;
    canWallJumpRight = false;
    canDoubleJump = true;
    releasedJump = true;
    airControlMultiplier = .01;
    newState = nullptr;
    this->view = &view;
    direction = glm::vec2(0,0);
    setUpSprite( resource );
    setUpBody();
    std::vector<Entity*> spawns = world->getEntitiesByType(Entity::Type::PlayerSpawn);
    if (spawns.size() >= 1 ) {
        ::PlayerSpawn* spawn = (::PlayerSpawn*)spawns[0];
        myBody->SetTransform(toB2(spawn->pos),0);
    }
    controllerID = 0;
    currentState = new IdleState(this);
}

void Player::setUpSprite( std::string resource ) {
    texture = Resources->getTexture(resource);
    // Walking animation
    dashingAnimation.setSpriteSheet(*texture);
    dashingAnimation.addFrame(sf::IntRect(335, 43, 27, 43));
    dashingAnimation.addFrame(sf::IntRect(364,43,28,43));
    dashingAnimation.addFrame(sf::IntRect(1,43,34,40));
    dashingAnimation.addFrame(sf::IntRect(449,1,32,40));
    dashingAnimation.addFrame(sf::IntRect(68,43,27,41));
    dashingAnimation.addFrame(sf::IntRect(37,43,29,41));
    dashingAnimation.addFrame(sf::IntRect(378,1,35,39));
    dashingAnimation.addFrame(sf::IntRect(415,1,32,39));
    idleAnimation.setSpriteSheet(*texture);
    idleAnimation.addFrame(sf::IntRect(165,43,32,42));
    idleAnimation.addFrame(sf::IntRect(199,43,32,42));
    idleAnimation.addFrame(sf::IntRect(131,43,32,42));
    idleAnimation.addFrame(sf::IntRect(267,43,32,42));
    idleAnimation.addFrame(sf::IntRect(233,43,32,42));
    idleAnimation.addFrame(sf::IntRect(301,43,32,42));
    idleAnimation.addFrame(sf::IntRect(97,43,32,42));
    airborneAnimation.setSpriteSheet(*texture);
    airborneAnimation.addFrame(sf::IntRect(426,43,31,44));
    airborneAnimation.addFrame(sf::IntRect(459,43,28,44));
    airborneAnimation.addFrame(sf::IntRect(394,43,30,44));
    jumpSquatAnimation.setSpriteSheet(*texture);
    jumpSquatAnimation.addFrame(sf::IntRect(169,1,29,35));
    jumpSquatAnimation.addFrame(sf::IntRect(239,1,27,37));
    runningAnimation.setSpriteSheet(*texture);
    runningAnimation.addFrame(sf::IntRect(342,1,34,39));
    runningAnimation.addFrame(sf::IntRect(200,1,37,37));
    runningAnimation.addFrame(sf::IntRect(45,1,43,33));
    runningAnimation.addFrame(sf::IntRect(129,1,38,35));
    runningAnimation.addFrame(sf::IntRect(307,1,33,38));
    runningAnimation.addFrame(sf::IntRect(268,1,37,37));
    runningAnimation.addFrame(sf::IntRect(1,1,42,32));
    runningAnimation.addFrame(sf::IntRect(90,1,37,34));

    sprite = new AnimatedSprite( sf::seconds(0.2), false, true );
    sprite->play( idleAnimation);
    sprite->scale(2,2);
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
    boxFixtureDef.friction = 1;
    boxFixtureDef.restitution = 0;
    myBody->CreateFixture(&boxFixtureDef);
    boxFixtureDef.shape = &circleShape;
    myBody->CreateFixture(&boxFixtureDef);
    circleShape.m_p.Set(0,-playerHeight/2.f);
    myBody->CreateFixture(&boxFixtureDef);
    flipped = false;
}

Player::~Player() {
    delete sprite;
}

void Player::draw( sf::RenderWindow& window ) {
    sf::Text stateText;
    stateText.setFont( *Resources->getFont( "assets/fonts/arial.ttf" ) );
    stateText.setString( StateString[currentState->getType()] );
    stateText.setCharacterSize(20);
    stateText.setStyle(sf::Text::Bold);
    //stateText.setColor(sf::Color::Red);
    stateText.setPosition(myBody->GetWorldCenter().x*64, myBody->GetWorldCenter().y*64);
    window.draw( *sprite );
    window.draw( stateText );
}

void Player::update( double dt ) {
    detectGround();
    detectWalls();

    /*for (int i=0;i<sf::Joystick::getButtonCount(0);i++ ) {
        if (sf::Joystick::isButtonPressed(0,i)) {
            std::cout << i << std::endl;
        }
    }*/

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
			direction += glm::vec2(0,1);
		}
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) || sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) {
			direction += glm::vec2(0,-1);
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

    // 25% deadzone
    if ( glm::length(direction) < deadZone ) {
        direction = glm::vec2(0,0);
    }

    currentState->update(this, dt);
    if (onGround ) {
        canDoubleJump = true;
    }

    glm::vec2 vel = toGLM(myBody->GetLinearVelocity());
    if ( vel.x > 0 && flipped) {
        sprite->scale(-1,1);
        flipped = false;
    } else if ( vel.x < 0 && !flipped ) {
        sprite->scale(-1,1);
        flipped = true;
    }

    lastDirection = direction;
    b2Vec2 pos = myBody->GetWorldCenter();
    float ang = myBody->GetAngle();
    // We'll assume 64 pixels is a meter
    if ( !flipped ) {
        sprite->setPosition( pos.x*64-35, pos.y*64-40 );
    } else {
        sprite->setPosition( pos.x*64+35, pos.y*64-40 );
    }
    sprite->setRotation( ang*180/3.149562 );
    view->setCenter( pos.x*64, pos.y*64 );
    sprite->update( sf::seconds(dt) );
    if ( newState ) {
        delete currentState;
        currentState = newState;
        newState = nullptr;
    }
}

void Player::detectGround() {
    b2AABB testAABB;
    b2Vec2 pos = myBody->GetWorldCenter();
    testAABB.lowerBound = b2Vec2(pos.x-playerWidth/3.f, pos.y);
    testAABB.upperBound = b2Vec2(pos.x+playerWidth/3.f, pos.y+playerHeight+0.1);
    MapQueryCallback queryCallback;
    physicalWorld->get().QueryAABB( &queryCallback, testAABB );
    onGround = queryCallback.foundMap;
    if (queryCallback.foundMap) {
        b2RayCastInput input;
        input.p1 = pos;
        input.p2 = pos+b2Vec2(0,playerHeight+0.1);
        input.maxFraction = 2;
        b2RayCastOutput output;
        for ( b2Fixture* f : queryCallback.hitFixtures ) {
            if( f->RayCast(&output,input,0) ) {
                groundHitNormal = toGLM(output.normal);
                //glm::vec2 hitPosition = toGLM(input.p1) + output.fraction * toGLM(input.p2-input.p1);
                //myBody->SetTransform(toB2(hitPosition-glm::vec2(0,playerHeight)), 0);
                break;
            }
        }
    }
}

void Player::detectWalls() {
    b2Vec2 pos = myBody->GetWorldCenter();

    b2AABB testAABB;
    testAABB.lowerBound = b2Vec2(pos.x, pos.y-playerHeight/2.f);
    testAABB.upperBound = b2Vec2(pos.x+playerWidth/2.f+0.1, pos.y+playerHeight/2.f);
    MapQueryCallback queryCallback;
    physicalWorld->get().QueryAABB( &queryCallback, testAABB );
    canWallJumpLeft = queryCallback.foundMap;

    b2AABB testAABB2;
    testAABB2.lowerBound = b2Vec2(pos.x-playerWidth/2.f-0.1, pos.y-playerHeight/2.f);
    testAABB2.upperBound = b2Vec2(pos.x, pos.y+playerHeight/2.f);
    MapQueryCallback queryCallback2;
    physicalWorld->get().QueryAABB( &queryCallback2, testAABB2 );
    canWallJumpRight = queryCallback2.foundMap;
}

void Player::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal ) {
}

Entity::Type Player::getType(){
    return Entity::Type::Player;
}

void Player::switchState( GenericPlayerState* state ) {
    newState = state;
}
