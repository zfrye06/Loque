#include "contactlistener.h"
#include "entity.h"

ContactListener::ContactListener()
{
    //ctor
}

ContactListener::~ContactListener()
{
    //dtor
}

void ContactListener::BeginContact(b2Contact* contact){
    void* bodyA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyB = contact->GetFixtureB()->GetBody()->GetUserData();

    b2WorldManifold worldManifold;
    contact->GetWorldManifold( &worldManifold );

    if(bodyA && bodyB){
        Entity* entityA = static_cast<Entity*>( bodyA );
        Entity* entityB = static_cast<Entity*>( bodyB );
        entityA->onHit(entityB, contact, worldManifold.normal);
        entityB->onHit(entityA, contact, worldManifold.normal);
    }
}
