#include "dust.h"

DashDust::DashDust( glm::vec2 pos, bool flipped, float angle) {
    texture = Resources->getTexture("assets/images/dash_dust.png");
    animation.setSpriteSheet(*texture);
    animation.addFrame(sf::IntRect(0,0,140,140));
    animation.addFrame(sf::IntRect(140,0,140,140));
    animation.addFrame(sf::IntRect(280,0,140,140));
    animation.addFrame(sf::IntRect(0,140,140,140));
    animation.addFrame(sf::IntRect(140,140,140,140));
    animation.addFrame(sf::IntRect(280,140,140,140));
    animation.addFrame(sf::IntRect(0,280,140,140));
    animation.addFrame(sf::IntRect(140,280,140,140));
    animation.addFrame(sf::IntRect(280,280,140,140));
    animation.addFrame(sf::IntRect(280,280,140,140));

    sprite = new AnimatedSprite( sf::seconds(0.07), false, false );
    sprite->setOrigin(0,76.f);
    sprite->setPosition( pos.x, pos.y );
    sprite->setRotation( angle );

    if ( !flipped ) {
        sprite->setScale( -0.7, 0.7 );
    } else {
        sprite->setScale( 0.7, 0.7 );
    }
    sprite->play(animation);
}

DashDust::~DashDust() {
    delete sprite;
}

void DashDust::update(double dt) {
    sprite->update( sf::seconds( dt ) );
    if ( !sprite->isPlaying() ) {
        world->removeEntity(this);
    }
}

void DashDust::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void DashDust::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}

Entity::Type DashDust::getType(){
    return Entity::Type::Dust;
}

JumpDust::JumpDust( glm::vec2 pos, float angle, bool doub ) {
    if ( !doub ) {
        texture = Resources->getTexture("assets/images/jump_poof2.png");
    } else {
        texture = Resources->getTexture("assets/images/walljump_poof.png");
    }
    animation.setSpriteSheet(*texture);
    animation.addFrame(sf::IntRect(0,0,128,128));
    animation.addFrame(sf::IntRect(128,0,128,128));
    animation.addFrame(sf::IntRect(256,0,128,128));
    animation.addFrame(sf::IntRect(0,128,128,128));
    animation.addFrame(sf::IntRect(128,128,128,128));
    animation.addFrame(sf::IntRect(256,128,128,128));
    animation.addFrame(sf::IntRect(0,256,128,128));
    animation.addFrame(sf::IntRect(128,256,128,128));
    animation.addFrame(sf::IntRect(256,256,128,128));

    sprite = new AnimatedSprite( sf::seconds(0.07), false, false );
    sprite->setOrigin(64,24.f);
    sprite->setPosition( pos.x, pos.y );
    sprite->setRotation( angle );
    sprite->setScale( 0.5, 0.5 );
    sprite->play(animation);
}

JumpDust::~JumpDust() {
    delete sprite;
}

void JumpDust::update(double dt) {
    sprite->update( sf::seconds( dt ) );
    if ( !sprite->isPlaying() ) {
        world->removeEntity(this);
    }
}

void JumpDust::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void JumpDust::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}

Entity::Type JumpDust::getType(){
    return Entity::Type::Dust;
}

WallJumpDust::WallJumpDust( glm::vec2 pos, bool flipped, float angle) {
    texture = Resources->getTexture("assets/images/walljump_poof2.png");
    animation.setSpriteSheet(*texture);
    animation.addFrame(sf::IntRect(0,0,128,128));
    animation.addFrame(sf::IntRect(128,0,128,128));
    animation.addFrame(sf::IntRect(256,0,128,128));
    animation.addFrame(sf::IntRect(0,128,128,128));
    animation.addFrame(sf::IntRect(128,128,128,128));
    animation.addFrame(sf::IntRect(256,128,128,128));
    animation.addFrame(sf::IntRect(0,256,128,128));
    animation.addFrame(sf::IntRect(128,256,128,128));
    animation.addFrame(sf::IntRect(256,256,128,128));

    sprite = new AnimatedSprite( sf::seconds(0.07), false, false );
    sprite->setOrigin(64.f,64.f);
    sprite->setPosition( pos.x, pos.y );
    sprite->setRotation( angle );

    if ( flipped ) {
        sprite->setScale( -0.7, 0.7 );
    } else {
        sprite->setScale( 0.7, 0.7 );
    }
    sprite->play(animation);
}

WallJumpDust::~WallJumpDust() {
    delete sprite;
}

void WallJumpDust::update(double dt) {
    sprite->update( sf::seconds( dt ) );
    if ( !sprite->isPlaying() ) {
        world->removeEntity(this);
    }
}

void WallJumpDust::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void WallJumpDust::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}

Entity::Type WallJumpDust::getType(){
    return Entity::Type::Dust;
}

LandingDust::LandingDust( glm::vec2 pos, float angle ) {
    texture = Resources->getTexture("assets/images/landing_poof.png");
    animation.setSpriteSheet(*texture);
    animation.addFrame(sf::IntRect(0,0,128,128));
    animation.addFrame(sf::IntRect(128,0,128,128));
    animation.addFrame(sf::IntRect(256,0,128,128));
    animation.addFrame(sf::IntRect(0,128,128,128));
    animation.addFrame(sf::IntRect(128,128,128,128));
    animation.addFrame(sf::IntRect(256,128,128,128));
    animation.addFrame(sf::IntRect(0,256,128,128));
    animation.addFrame(sf::IntRect(128,256,128,128));
    animation.addFrame(sf::IntRect(256,256,128,128));

    sprite = new AnimatedSprite( sf::seconds(0.07), false, false );
    sprite->setOrigin(64,38.f);
    sprite->setPosition( pos.x, pos.y );
    sprite->setRotation( angle );
    sprite->setScale( 0.7, 0.7 );
    sprite->play(animation);
}

LandingDust::~LandingDust() {
    delete sprite;
}

void LandingDust::update(double dt) {
    sprite->update( sf::seconds( dt ) );
    if ( !sprite->isPlaying() ) {
        world->removeEntity(this);
    }
}

void LandingDust::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void LandingDust::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}

Entity::Type LandingDust::getType(){
    return Entity::Type::Dust;
}
