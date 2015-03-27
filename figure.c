//
//  figure.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 25/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include <stdio.h>
#include "figure.h"

size_t nb_points_carre(int cote, int centre_x, int centre_y){
    cote = (cote % 2 == 0) ? cote+1 : cote;
    return 4*(cote-1);
}

size_t nb_point_rectangle(int largeur, int longueur, int centre_x, int centre_y){
    largeur = (largeur % 2 == 0) ? largeur+1 : largeur;
    longueur = (longueur % 2 == 0) ? longueur+1 : longueur;
    return 2*longueur+2*(largeur-2);
}

size_t nb_point_cercle(int rayon){
    return 8*rayon;
}

size_t nb_point_segment(int x1, int y1, int x2, int y2){
    return max(abs(x1-x2)+1,abs(y1-y2)+1);
}
