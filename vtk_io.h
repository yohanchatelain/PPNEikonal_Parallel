//
//  vtk_io.h
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#ifndef __PPNEikonal_Parallel__vtk_io__
#define __PPNEikonal_Parallel__vtk_io__

#include <stdio.h>
#include "grid.h"

void writeVTK(Pnum *grid ,size_t N, char *file_name_matrix);

#endif /* defined(__PPNEikonal_Parallel__vtk_io__) */
