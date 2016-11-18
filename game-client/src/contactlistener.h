#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>


class ContactListener : public b2ContactListener
{
    public:
        ContactListener();
        virtual ~ContactListener();

        void BeginContact(b2Contact* contact);

    protected:

    private:
};

#endif // CONTACTLISTENER_H
