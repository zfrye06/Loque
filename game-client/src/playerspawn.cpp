#include "playerspawn.h"

PlayerSpawn::PlayerSpawn(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x/64+obj.getAABB().width/128.f,obj.getPosition().y/64+obj.getAABB().height/128.f);
}

void PlayerSpawn::update( double dt){
}

void PlayerSpawn::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void PlayerSpawn::draw(sf::RenderTarget& window){
}

Entity::Type PlayerSpawn::getType(){
    return Entity::Type::PlayerSpawn;
}
