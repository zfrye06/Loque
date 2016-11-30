#ifndef LQ_RANDOM_H_
#define LQ_RANDOM_H_

#include <cmath>
#include <stdlib.h>
#include <time.h>

class RandomClass {
public:
    RandomClass();
    float f( float lower, float upper );
};

extern RandomClass* Random;

#endif
