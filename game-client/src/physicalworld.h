#ifndef LQ_PHYSICALWORLD_H_
#define LQ_PHYSICALWORLD_H_

#include <Box2D/Box2D.h>
#include "contactlistener.h"

class ContactListener;

class PhysicalWorld {
private:
    double timer;
    b2World* world;
public:
    PhysicalWorld();
    ~PhysicalWorld();
    void step( double dt );
    b2World& get();
    ContactListener* contactlistenerinstance;
};

extern PhysicalWorld* physicalWorld;

#endif
