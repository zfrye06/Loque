/**
 * @file contactlistener.h
 * @brief Actually runs the onHit function on entities, and adds a MapQueryCallback to look for the map.
 * @author Zach Frye
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_CONTACTLISTENER_H
#define LQ_CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include <vector>
#include "entity.h"

class ContactListener : public b2ContactListener
{
    public:
        ContactListener();
        virtual ~ContactListener();

        void BeginContact(b2Contact* contact);

    protected:

    private:
};

class MapQueryCallback : public b2QueryCallback {
public:
    MapQueryCallback();
    bool foundMap;
    std::vector<b2Fixture*> hitFixtures;
    bool ReportFixture(b2Fixture* fixture);
};

#endif // CONTACTLISTENER_H
