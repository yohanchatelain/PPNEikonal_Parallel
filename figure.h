//
//  figure.h
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#ifndef __PPNEikonal_Parallel__figure__
#define __PPNEikonal_Parallel__figure__

#define max(a,b) (((a)<(b)) ? (b) : (a))

#include <stdlib.h>

enum Figure {Points, Segment, Circle, Square, Rectangle, Gaussian, Disk, Random};

size_t nb_points_carre(int cote, int centre_x, int centre_y);
size_t nb_point_rectangle(int largeur, int longueur, int centre_x, int centre_y);
size_t nb_point_cercle(int rayon);
size_t nb_point_segment(int x1, int y1, int x2, int y2);

#endif
