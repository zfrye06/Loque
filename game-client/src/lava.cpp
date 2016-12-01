#include "lava.h"

Lava::Lava(tmx::Object& obj){
        
    pos = glm::vec2(obj.getPosition().x, obj.getPosition().y);

    texture = Resources->getTexture("assets/images/lava.png");
    animation.setSpriteSheet(*texture);
    animation.addFrame(sf::IntRect(0,0,128,128));
    animation.addFrame(sf::IntRect(128,0,128,128));
    animation.addFrame(sf::IntRect(128*2,0,128,128));
    animation.addFrame(sf::IntRect(128*3,0,128,128));
    animation.addFrame(sf::IntRect(128*4,0,128,128));
    animation.addFrame(sf::IntRect(128*5,0,128,128));
    animation.addFrame(sf::IntRect(128*6,0,128,128));
    animation.addFrame(sf::IntRect(0,128,128,128));
    animation.addFrame(sf::IntRect(128,128,128,128));
    animation.addFrame(sf::IntRect(128*2,128,128,128));
    animation.addFrame(sf::IntRect(128*3,128,128,128));

    surfaceTexture= Resources->getTexture("assets/images/lava_surface.png");
    surfaceAnimation.setSpriteSheet(*surfaceTexture);
    surfaceAnimation.addFrame(sf::IntRect(0,0,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*2,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*3,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*4,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*5,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*6,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*7,128,12));
    surfaceAnimation.addFrame(sf::IntRect(0,12*8,128,12));

    rect = obj.getAABB();

    tmx::FloatRect size = obj.getAABB();

    surfaceSprite = new AnimatedSprite( sf::seconds(0.15), false, true );
    surfaceSprite->play( surfaceAnimation );
    sprite = new AnimatedSprite( sf::seconds(0.15), false, true );
    sprite->play( animation );

    size.width = size.width / 64;
    size.height = size.height / 64;
    size.left = size.left / 64;
    size.top = size.top / 64;

    impulseMultiplier = 30.f;
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
    body->CreateFixture(&fixDef);
    body->SetUserData( this );
}

Lava::~Lava() {
    delete surfaceSprite;
    delete sprite;
}

void Lava::update(double dt){
    sprite->update( sf::seconds( dt ) );
    surfaceSprite->update( sf::seconds( dt ) );
}

void Lava::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
    if(collider->getType() == Entity::Type::Player){
        ::Player* p = static_cast< ::Player*>(collider);
        if ( !p->isDamageBoosted() ) {
            playerStats->setScore(playerStats->getScore()-1);
            p->damageBoost();
            p->canDoubleJump = true;
            world->addEntity( new PokeDust( p->position + glm::vec2(0.f,32.f)), World::Layer::Foreground );
            world->stutter(p->hitLength/2.f,0.1);
            p->shake(10,p->hitLength,0.1);
            p->switchState( new ShockedState( p, toGLM(hitnormal)*impulseMultiplier, 1, p->hitLength) );
        }
    }
}

void Lava::draw(sf::RenderTarget& window){
    //window.draw(verticies.data(), verticies.size(), sf::Quads);
    for ( float x = rect.left;x<rect.left+rect.width;x+=128 ) {
        float w = 1;
        if ( rect.left+rect.width - x < 128 ) {
            w = (rect.left+rect.width - x)/128.f;
        }
        surfaceSprite->setPosition( x, rect.top-12 );
        surfaceSprite->setScale( w, 1 );
        window.draw(*surfaceSprite);
    }
    for ( float x = rect.left;x<rect.left+rect.width;x+=128 ) {
        for ( float y = rect.top;y<rect.top+rect.height;y+=128 ) {
            float w = 1;
            float h = 1;
            if ( rect.top+rect.height - y < 128 ) {
                h = (rect.top+rect.height - y)/128.f;
            }
            if ( rect.left+rect.width - x < 128 ) {
                w = (rect.left+rect.width - x)/128.f;
            }
            sprite->setPosition( x, y );
            sprite->setScale( w, h );
            window.draw(*sprite);
        }
    }
}

Entity::Type Lava::getType(){
    return Entity::Type::Lava;
}
