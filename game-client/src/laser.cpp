#include "laser.h"

Laser::Laser(tmx::Object& obj){
    fireworkWait = 0;
    fireworkTimer = 0;
    sound = sf::Sound( *Resources->getSound( "assets/audio/effects/birthday.ogg" ) );
    passed = false;
    std::vector<tmx::Property> properties = obj.getProperties();
    for(unsigned int i = 0; i < properties.size(); i++) {
        tmx::Property temp = properties[i];
        if(temp.getName() == "canBePassed"){
            canBePassed = temp.getBoolValue();
        }
    }

    impulseMultiplier = 30.f;

    tmx::FloatRect size = obj.getAABB();
    pos = glm::vec2(obj.getPosition().x+size.width/2.f, obj.getPosition().y+size.height/2.f); 
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
    body = physicalWorld->get().CreateBody( &boxDef );
    b2Fixture* fixture = body->CreateFixture( &fixDef );
    if ( canBePassed ) {
        fixture->SetSensor(true);
    }
    body->SetUserData( this );
}

void Laser::update( double dt ){
    if (fireworkTimer > 0 ) {
        if ( fireworkWait < 0 ) {
            glm::vec2 rand = pos + glm::vec2( Random->f(-100.f,100.f), Random->f(-100.f,100.f) );
            world->addEntity( new FireworkDust( rand ), World::Layer::Foreground );
            fireworkWait = Random->f(0.2f,0.3f);
        } else {
            fireworkWait -= dt;
        }
        fireworkTimer -= dt;
    }
}

void Laser::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal ){
    if(collider->getType() == Entity::Type::Player){
        ::Player* p = static_cast< ::Player*>( collider );
        if(!this->canBePassed && !p->isDamageBoosted()){
            playerStats->setScore(playerStats->getScore()-1);
            p->canDoubleJump = true;
            p->damageBoost();
            world->addEntity( new ShockDust( p->position ), World::Layer::Foreground );
            world->stutter(p->shockLength/2.f,0.1);
            p->shake(10,p->shockLength,0.1);
            p->switchState( new ShockedState( p, glm::normalize(toGLM(hitnormal))*impulseMultiplier, 0, p->shockLength) );
        }
        if (this->canBePassed && !passed ) {
            fireworkTimer = 1.7;
            sound.play();
            passed = true;
            world->addEntity( new FireworkDust( p->position ), World::Layer::Foreground );
            playerStats->setScore( playerStats->getScore() + 10 );
        }
    }
}

void Laser::draw( sf::RenderTarget& window ){
}

Entity::Type Laser::getType(){
    return Entity::Type::Laser;
}
