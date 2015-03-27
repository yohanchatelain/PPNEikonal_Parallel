//
//  solver.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "solver.h"

Pnum solveEquation_1(Pnum Tx, Pnum Ty,Pnum f){
    if (fabs(Tx-Ty) >= sqrtf(2.0)/f)
        return fminf(Tx, Ty) + 1/f;
    else
        return (Tx+Ty+sqrtf(2/(f*f)-(Tx-Ty)*(Tx-Ty)))/2.0;
}

Pnum solveEquation_2(Pnum Tx, Pnum Ty,Pnum f){

    Pnum a, b, c;
    
    a = 9/4.0 * (((Tx != 0) ? 1 : 0) + ((Ty != 0) ? 1 : 0));
    b = (-9/4.0)*(Tx + Ty);
    c = (9/4.0)*(Tx*Tx + Ty*Ty) - 1/(f*f);

    Pnum delta = b*b - 4*a*c;
  
    if (delta < 0)
        return INFTY;
    else
        return -b+sqrtf(delta)/(2*a);
}

