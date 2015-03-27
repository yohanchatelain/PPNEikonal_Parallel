//
//  measure.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 27/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "measure.h"

#ifdef __i386
    extern __inline__ uint64_t rdtsc(void) {
        uint64_t x;
        __asm__ volatile ("rdtsc" : "=A" (x));
        return x;
    }
#elif defined __amd64
    extern __inline__ uint64_t rdtsc(void) {
        uint64_t a, d;
        __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
        return (d<<32) | a;
    }
#endif

size_t get_time(){
    return rdtsc()/CLOCK_HZ;
}