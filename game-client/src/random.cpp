#include "random.h"

RandomClass* Random;

RandomClass::RandomClass() {
    srand(time(NULL));
}

// Can you believe this function used to only return 0?
// It was pretty random in the sense I couldn't figure out why the position of entities kept turning into NAN's.
float RandomClass::f( float lower, float upper ) {
    float d = upper-lower;
    int r = rand()%RAND_MAX;
    float scale = (float)r/(float)RAND_MAX;
    return lower+d*scale;
}
