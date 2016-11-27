#include "entity.h"
#include <vector>
#include <iostream>

Entity::Entity() {
}
Entity::~Entity() {
}
void Entity::update( double dt ) {
}
void Entity::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal  ) {
}
void Entity::draw( sf::RenderWindow& window ) {
}
Entity::Type Entity::getType(){
    return Entity::Type::None;
}
PhysicsDebug::PhysicsDebug(sf::RenderWindow& window) {
    uint32 flags = b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;

    drawer = new DebugDraw(window);
    drawer->SetFlags(flags);
    physicalWorld->get().SetDebugDraw(drawer);
}
PhysicsDebug::~PhysicsDebug(){}
void PhysicsDebug::update( double dt ){}
void PhysicsDebug::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal  ){}
void PhysicsDebug::draw( sf::RenderWindow& window ) {
    window.pushGLStates();
    physicalWorld->get().DrawDebugData();
    window.popGLStates();
}

Entity::Type PhysicsDebug::getType(){
    return Entity::Type::None;
}

Background::Background(sf::View* view, std::string skyFile, std::string cloudsFile, std::string hillsFile){
    defaultView = view;
    sky = sf::Sprite(*Resources->getTexture(skyFile));
    clouds = sf::Sprite(*Resources->getTexture(cloudsFile));
    hills = sf::Sprite(*Resources->getTexture(hillsFile));
    Resources->getTexture(skyFile)->setRepeated(true);
    Resources->getTexture(cloudsFile)->setRepeated(true);
    Resources->getTexture(hillsFile)->setRepeated(true);
}

void Background::update( double dt){
    glm::vec2 viewPos = toGLM(defaultView->getCenter());
    sf::IntRect hillUV = hills.getTextureRect();
    hillUV.left = viewPos.x/4;
//    hillUV.top += viewPos.y/280;
    hills.setTextureRect(hillUV);
    sf::IntRect cloudsUV = clouds.getTextureRect();
    cloudsUV.left = viewPos.x/8;
    clouds.setTextureRect(cloudsUV);
}

void Background::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Background::draw(sf::RenderWindow& window){
    
    sf::View view(sf::FloatRect(0,0,800,600));
    window.setView(view);
    window.draw(sky);
    window.draw(clouds);
    window.draw(hills);
    window.setView(*defaultView);
}

Entity::Type Background::getType(){
    return Entity::Type::None;
}

PlayerSpawn::PlayerSpawn(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x/64,obj.getPosition().y/64);
}

void PlayerSpawn::update( double dt){
}

void PlayerSpawn::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void PlayerSpawn::draw(sf::RenderWindow& window){
}

Entity::Type PlayerSpawn::getType(){
    return Entity::Type::PlayerSpawn;
}
