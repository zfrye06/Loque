#include "respawn.h"

Respawn::Respawn(glm::vec2 pos, sf::View& viewTemp){
    position = pos;
    view = &viewTemp;
    view->setCenter(pos.x*64, pos.y*64);
    //delete all players
    std::vector<Entity*> players = world->getEntitiesByType(Entity::Type::Player);
    for(int i = 0; i < players.size(); i++){
        world->removeEntity(players[i]);
    }

    texture = Resources->getTexture("assets/images/respawn.png");
    animation.setSpriteSheet(*texture);
    float w = 192;
    float h = 192;
    int frames = 20;
    int columns = 5;
    int rows = 4;
    int y = 0;
    int curframe = 0;
    for (int y=0;y<rows&&curframe < frames;y++ ) {
        for (int x=0;x<columns&&curframe < frames;x++ ) {
            animation.addFrame(sf::IntRect(x*w,y*h,w,h));
            curframe++;
        }
    }

    sprite = new AnimatedSprite( sf::seconds(0.05), false, false );
    sprite->setOrigin(w/2.f,h/2.f);
    sprite->setPosition( pos.x*64, pos.y*64 );
    //sprite->setScale( 1, 1 );
    sprite->play(animation);
}

Respawn::~Respawn(){
    delete sprite;
}

void Respawn::update(double dt){
    view->setCenter(position.x*64, position.y*64);
    sprite->update( sf::seconds(dt) );
    if( !sprite->isPlaying() ){
        world->addEntity(new ::Player("assets/images/veemon.png",position, *view));
        world->removeEntity(this);
    }
}

void Respawn::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Respawn::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}

Entity::Type Respawn::getType(){
    return Entity::Type::RespawnAnim;
}
