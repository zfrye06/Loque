#ifndef LQ_RANDOM_H_
#define LQ_RANDOM_H_

#include <stdlib.h>
#include <time.h>
#include <cmath>

class RandomClass {
public:
    RandomClass();
    float f( float lower, float upper );
};

extern RandomClass* Random;

#endif
