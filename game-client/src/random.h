/**
 * @file random.h
 * @brief Imagine using a language which doesn't have any built in randomization functionality. OH WAIT
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */

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
