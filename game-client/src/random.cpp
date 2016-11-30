#include "random.h"

RandomClass* Random;

RandomClass::RandomClass() {
    srand(time(NULL));
}

float RandomClass::f( float lower, float upper ) {
    float d = upper-lower;
    return lower+(float)(fmod(rand(),d))/(float)d;
}
