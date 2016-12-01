#include "random.h"

RandomClass* Random;

RandomClass::RandomClass() {
    srand(time(NULL));
}

float RandomClass::f( float lower, float upper ) {
    float d = upper-lower;
    int r = rand()%RAND_MAX;
    float scale = (float)r/(float)RAND_MAX;
    return lower+d*scale;
}
