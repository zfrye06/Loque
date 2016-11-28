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

ShockDust::ShockDust( glm::vec2 pos ) {
    if ( Random->f(0,1) > 0.5 ) {
        sound = sf::Sound( *Resources->getSound( "assets/audio/effects/shock1.ogg" ) );
    } else {
        sound = sf::Sound( *Resources->getSound( "assets/audio/effects/shock2.ogg" ) );
    }
    sound.play();
    texture = Resources->getTexture("assets/images/lightning.png");
    animation.setSpriteSheet(*texture);
    float w = 96;
    float h = 96;
    int frames = 10;
    int columns = 5;
    int rows = 2;
    int y = 0;
    int curframe = 0;
    for (int y=0;y<rows&&curframe < frames;y++ ) {
        for (int x=0;x<columns&&curframe < frames;x++ ) {
            animation.addFrame(sf::IntRect(x*w,y*h,w,h));
            curframe++;
        }
    }

    sprite = new AnimatedSprite( sf::seconds(0.07), false, false );
    sprite->setOrigin(w/2.f,h/2.f);
    sprite->setPosition( pos.x, pos.y );
    //sprite->setScale( 1, 1 );
    sprite->play(animation);
}

ShockDust::~ShockDust() {
    delete sprite;
}

void ShockDust::update(double dt) {
    sprite->update( sf::seconds( dt ) );
    if ( !sprite->isPlaying() && sound.getStatus() != sf::SoundSource::Playing ) {
        world->removeEntity(this);
    }
}

void ShockDust::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void ShockDust::draw(sf::RenderWindow& window){
    if ( sprite->isPlaying() ) {
        window.draw(*sprite);
    }
}

Entity::Type ShockDust::getType(){
    return Entity::Type::Dust;
}

PokeDust::PokeDust( glm::vec2 pos ) {
    sound = sf::Sound( *Resources->getSound( "assets/audio/effects/stab1.ogg" ) );
    sound.play();
    texture = Resources->getTexture("assets/images/shock_wave.png");
    animation.setSpriteSheet(*texture);
    animation.addFrame(sf::IntRect(0,0,127,136));
    animation.addFrame(sf::IntRect(127,0,127,136));
    animation.addFrame(sf::IntRect(127*2,0,127,136));

    sprite = new AnimatedSprite( sf::seconds(0.05), false, false );
    sprite->setOrigin(127.f/2.f,136.f/2.f);
    sprite->setPosition( pos.x, pos.y );
    sprite->setScale( 0.7, 0.7 );
    sprite->play(animation);
}

PokeDust::~PokeDust() {
    delete sprite;
}

void PokeDust::update(double dt) {
    sprite->update( sf::seconds( dt ) );
    if ( !sprite->isPlaying() && sound.getStatus() != sf::SoundSource::Playing ) {
        world->removeEntity(this);
    }
}

void PokeDust::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void PokeDust::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}

Entity::Type PokeDust::getType(){
    return Entity::Type::Dust;
}
