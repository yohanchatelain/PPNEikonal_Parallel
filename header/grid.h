//
//  grid.h
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#ifndef __PPNEikonal_Parallel__grid__
#define __PPNEikonal_Parallel__grid__

#include <stdio.h>
#include <string.h>
#include "parametre.h"

typedef struct {
    size_t N;
    size_t X;
    size_t Y;
    Pnum * grid;
    Pnum * velocity;
    size_t rank;
    size_t size;
    char file_name[64];
}grid_t;

#endif /* defined(__PPNEikonal_Parallel__grid__) */
