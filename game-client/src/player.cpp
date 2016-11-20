#include "player.h"

Player::Player( std::string resource, sf::View& view ) {
    playerWidth = .7;
    playerHeight = .6;
    playerSpeed = 15;
    fullHopHeight = 8000;
    shortHopHeight = 4000;
    airControlMultiplier = 0.5;
    jumps = 0;
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

    b2Vec2 pos = myBody->GetWorldCenter();
    float ang = myBody->GetAngle();
    // We'll assume 64 pixels is a meter
    sprite->setPosition( pos.x*64-35, pos.y*64-50 );
    sprite->setRotation( ang*180/3.149562 );
    view->setCenter( pos.x*64, pos.y*64 );
    sprite->update( sf::seconds(dt) );
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
    // If we hit a map...
    if ( collider->getType() == Entity::Type::Map ) {
        // If the degrees between up and the hitnormal is within pi/5ths...
        if ( fabs(atan( hitnormal.x )) < M_PI/5.f ) {
            // Then we can jump!
            jumps = 2;
        }
    }
}

Entity::Type Player::getType(){
    return Entity::Type::Player;
}

