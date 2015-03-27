//
//  grid.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "grid.h"

static Pnum * init_node(grid_t * grid){
    Pnum * grid_temp;
    
    grid_temp = malloc( (grid->X)*(grid->Y+4) * sizeof(Pnum));
    memset(grid_temp, INFTY, (grid->X)*(grid->Y+4)*sizeof(Pnum));
    
    return grid_temp;
}

void init_MPI(size_t rank, size_t size, grid_t * grid){
    grid->rank = rank;
    grid->size = size;
}
