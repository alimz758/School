//
//  randlibhw.c
//  
//
//  Created by ali mirabzadeh on 3/2/19.
//


#include "randlib.h"
#include <immintrin.h>

/* Return a random value, using hardware operations.  */
static unsigned long long
hardware_rand64 (void)
{
    unsigned long long int x;
    while (! _rdrand64_step (&x))
        continue;
    return x;
}

extern unsigned long long rand64 (void) {
    return hardware_rand64();
}
