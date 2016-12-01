#include "respawn.h"

Respawn::Respawn(glm::vec2 pos){
    sound = sf::Sound( *Resources->getSound( "assets/audio/effects/teleport.ogg" ));
    sound.play();

    position = pos;
    world->view.setCenter(pos.x*64,pos.y*64);

    //delete all players
    std::vector<Entity*> players = world->getEntitiesByType(Entity::Type::Player);
    for(uint i = 0; i < players.size(); i++){
        world->removeEntity(players[i], World::Layer::Midground);
    }

    texture = Resources->getTexture("assets/images/respawn.png");
    animation.setSpriteSheet(*texture);
    float w = 192;
    float h = 192;
    int frames = 20;
    int columns = 5;
    int rows = 4;
    int curframe = 0;
    for (int y=0;y<rows&&curframe < frames;y++ ) {
        for (int x=0;x<columns&&curframe < frames;x++ ) {
            animation.addFrame(sf::IntRect(x*w,y*h,w,h));
            curframe++;
        }
    }

    sprite = new AnimatedSprite( sf::seconds(0.09), false, false );
    sprite->setOrigin(w/2.f,h/2.f);
    sprite->setPosition( pos.x*64, pos.y*64 );
    sprite->play(animation);
}

Respawn::~Respawn(){
    delete sprite;
}

void Respawn::update(double dt){
    world->view.setCenter(position.x*64,position.y*64);
    sprite->update( sf::seconds(dt) );
    if ( !sprite->isPlaying() && sound.getStatus() != sf::SoundSource::Playing ) {
        world->addEntity(new ::Player("assets/images/veemon.png",position), World::Layer::Midground);
        world->removeEntity(this, World::Layer::Background);
    }
}

void Respawn::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Respawn::draw(sf::RenderTarget& window){
    window.draw(*sprite);
}

Entity::Type Respawn::getType(){
    return Entity::Type::RespawnAnim;
}
