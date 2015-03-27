//
//  velocity.h
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#ifndef __PPNEikonal_Parallel__velocity__
#define __PPNEikonal_Parallel__velocity__

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "figure.h"
#include "grid.h"

typedef struct {
    enum Figure fig;
    Pnum v;
    size_t center_x,center_y;
    size_t length,width;
    size_t radius;
    size_t x1,y1;
    size_t x2,y2;
    Pnum sigma_x,sigma_y;
    size_t nb_points;
}velocity_t;

void init_velocity(grid_t *grid, velocity_t *velocity);

//void init_velocity_const(grid_t *grid, velocity_t *velocity);
//void init_velocity_gaussian(grid_t *grid, velocity_t *velocity);
//void init_velocity_random(grid_t *grid, velocity_t *velocity);
//void init_velocity_square(grid_t *grid,velocity_t *velocity);
//void init_velocity_rectangle(grid_t *grid,velocity_t *velocity);
//void init_velocity_circle(grid_t *grid, velocity_t *velocity);
//void init_velocity_disk(grid_t *grid, velocity_t *velocity);


#endif /* defined(__PPNEikonal_Parallel__velocity__) */
