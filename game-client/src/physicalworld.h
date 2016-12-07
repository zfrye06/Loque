/**
 * @file physicalworld.h
 * @brief Makes an ugly global pointer to box2d, and hooks up the contact listener. It used to also keep the physics updating in discrete chunks of time, but now world handles that.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_PHYSICALWORLD_H_
#define LQ_PHYSICALWORLD_H_

#include <Box2D/Box2D.h>
#include "contactlistener.h"

class ContactListener;

class PhysicalWorld {
private:
    //double timer;
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
