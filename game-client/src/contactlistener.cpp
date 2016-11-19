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

    if(bodyA && bodyB){
        Entity* entityA = static_cast<Entity*>( bodyA );
        Entity* entityB = static_cast<Entity*>( bodyB );
        entityA->onHit(entityB, contact);
        entityB->onHit(entityA, contact);
    }
}
