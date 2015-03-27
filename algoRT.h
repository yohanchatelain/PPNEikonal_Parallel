//
//  algoRT.h
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#ifndef __PPNEikonal_Parallel__algoRT__
#define __PPNEikonal_Parallel__algoRT__

#include <stdio.h>
#include "math.h"
#include "grid.h"
#include "front.h"
#include "solver.h"
#include "vtk_io.h"
#include </usr/local/include/mpi.h>

void RT_firstOrder(grid_t *grid);
void RT_secondOrder(grid_t *grid);

#endif /* defined(__PPNEikonal_Parallel__algoRT__) */
