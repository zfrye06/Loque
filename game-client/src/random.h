#ifndef LQ_RANDOM_H_
#define LQ_RANDOM_H_

#include <stdlib.h>
#include <time.h>

class RandomClass {
public:
    RandomClass() {
        srand(time(NULL));
    }
    float f( float lower, float upper ) {
        float d = upper-lower;
        return lower+(float)(fmod(rand(),d))/(float)d;
    }
    
};

extern RandomClass* Random;

#endif
