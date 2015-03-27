//
//  front.h
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#ifndef __PPNEikonal_Parallel__front__
#define __PPNEikonal_Parallel__front__

#include <stdio.h>
#include "figure.h"
#include "grid.h"

typedef struct {
    enum Figure fig;
    size_t center_x,center_y;
    size_t length,width;
    size_t radius;
    size_t x1,y1;
    size_t x2,y2;
    size_t nb_points;
    size_t *points;
}front_t;

void init_front(grid_t * grid, front_t * front);

//void init_front_points(grid_t * grid, front_t *front);
//void init_front_square(grid_t * grid, front_t *front);
//void init_front_rectangle(grid_t * grid, front_t *front);
//void init_front_circle(grid_t * grid,front_t *front);
//void init_front_segment(grid_t *grid, front_t *front);

#endif /* defined(__PPNEikonal_Parallel__front__) */

