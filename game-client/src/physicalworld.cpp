#include "physicalworld.h"

PhysicalWorld* physicalWorld = new PhysicalWorld();

PhysicalWorld::PhysicalWorld() {
    world = new b2World( b2Vec2( 0, 18 ) );
    contactlistenerinstance = new ContactListener();
    world->SetContactListener(contactlistenerinstance);
}

PhysicalWorld::~PhysicalWorld() {
    delete world;
    delete contactlistenerinstance;
}

b2World& PhysicalWorld::get() {
    return *world;
}

void PhysicalWorld::step( double dt ) {
    timer += dt;
    // Physics will run at 60fps.
    double step = 1.f/60.f;
    while ( timer > step ) {
        world->Step(step,8,3);
        timer -= step;
    }
}
