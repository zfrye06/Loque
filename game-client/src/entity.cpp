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
    //flags += b2Draw::e_aabbBit;
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
//    hillUV.top += viewPos.y/256;
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

Laser::Laser(tmx::Object& obj){
    pos = glm::vec2(obj.getPosition().x/64, obj.getPosition().y/64); 

    std::vector<tmx::Property> properties = obj.getProperties();
    for(int i = 0; i < properties.size(); i++) {
        tmx::Property temp = properties[i];
        if(temp.getName() == "canBePassed"){
            canBePassed = temp.getBoolValue();
        }
    }

    tmx::FloatRect size = obj.getAABB();
    size.width = size.width / 64;
    size.height = size.height / 64;
    size.left = size.left / 64;
    size.top = size.top / 64;

    b2Vec2 topLeft(size.left, size.top);
    b2Vec2 topRight(size.left + size.width, size.top);
    b2Vec2 bottomRight(size.left + size.width, size.top + size.height);
    b2Vec2 bottomLeft(size.left, size.top + size.height);

    b2BodyDef boxDef;
    boxDef.type = b2_staticBody;
    boxDef.angle = 0;

    b2PolygonShape shape;
    b2Vec2 vertices[] = {
        topLeft,
        topRight,
        bottomRight,
        bottomLeft
    };
    shape.Set(vertices, 4);
    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.density = 0;
    fixDef.restitution = 0;
    boxDef.position.Set(0,0);
    b2Body* body = physicalWorld->get().CreateBody( &boxDef );
    b2Fixture* fixture = body->CreateFixture( &fixDef );
    fixture->SetSensor(true);
    body->SetUserData( this );
}

void Laser::update( double dt ){
}

void Laser::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal ){
}

void Laser::draw( sf::RenderWindow& window ){
}

Entity::Type Laser::getType(){
    return Entity::Type::Laser;
}
