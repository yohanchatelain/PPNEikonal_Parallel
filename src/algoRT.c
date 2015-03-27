//
//  algoRT.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "algoRT.h"

void RT_firstOrder(grid_t *grid){
    
    int tag = 1000;
    MPI_Request req[2];
    MPI_Status sta[2];
    
    int convergence, convergence_all;
    size_t x,y;
    size_t nb_iter = 0;
    Pnum x_m1,x_p1,y_m1,y_p1,Tx,Ty,f,sol,sol_pred;
    do{
        convergence = 1;

        if (grid->rank == 0) {
            MPI_Isend(grid->grid+grid->X*grid->Y,(int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req);
        } else if (grid->rank == grid->size-1) {
            MPI_Isend(grid->grid+(2*grid->X), (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req+1);
        } else {
            MPI_Isend(grid->grid+grid->X*grid->Y, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req);
            MPI_Isend(grid->grid+(2*grid->X), (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req+1);
        }
        # pragma omp parallel for
        for (y = 0; y<grid->Y; y++) {
            for (x = 0; x<grid->X; x++) {
                if (grid->velocity[ (y+2)*grid->X + x ] < 0){
                    grid->grid[ (y+2)*grid->X + x] = INFTY;
                    continue;
                }
                
                x_m1   = (x >= 1)  ?   grid->grid[ (y+2)*grid->X + x-1]  :   INFTY;
                x_p1   = (x+1 < grid->X) ? grid->grid[ (y+2)*grid->X + x+1] : INFTY;
                y_m1   = grid->grid[ (y+1)*grid->X + x];
                y_p1   = grid->grid[ (y+3)*grid->X + x];
                Tx     = fminf(x_m1, x_p1);
                Ty     = fminf(y_m1, y_p1);
                f      = grid->velocity[ (y+2)*grid->X + x];
                sol_pred = grid->grid[ (y+2)*grid->X + x];
                sol     = fminf(sol_pred,solveEquation_1(Tx,Ty,f));
                grid->grid[ (y+2)*grid->X + x] = sol;
                
                convergence = convergence && (fabsf(sol - sol_pred) <= EPSILON);
                
            }
        }
        if (grid->rank == 0) {
            MPI_Irecv(grid->grid+grid->X*grid->Y, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req+1);
            MPI_Waitall(1, req, sta);
        } else if (grid->rank == grid->size-1) {
            MPI_Irecv(grid->grid, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req);
            MPI_Waitall(1, req, sta);
        } else {
            MPI_Irecv(grid->grid+grid->X*grid->Y, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req+1);
            MPI_Irecv(grid->grid, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req);
            MPI_Waitall(2, req, sta);
        }
        MPI_Allreduce(&convergence, &convergence_all, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        nb_iter++;
    } while (!convergence_all);
    
    Pnum *rcvbuf = NULL;
    if (grid->rank == 0) {
        rcvbuf = (Pnum*) malloc(grid->N*grid->N*sizeof(Pnum));
        MPI_Gather(grid->grid,(int)(grid->X*grid->Y), MPI_FLOAT, rcvbuf,(int)( grid->X*grid->Y), MPI_FLOAT, 0, MPI_COMM_WORLD);
        writeVTK(rcvbuf, grid->N, grid->file_name);
    }else{
        MPI_Gather(grid->grid,(int)(grid->X*grid->Y), MPI_FLOAT, rcvbuf,(int)( grid->X*grid->Y), MPI_FLOAT, 0, MPI_COMM_WORLD);
    }
}

void RT_secondOrder(grid_t *grid){
    
    int tag = 1000;
    MPI_Request req[2];
    MPI_Status sta[2];
    
    int convergence, convergence_all;
    size_t x,y;
    size_t nb_iter = 0;
    Pnum x_m1, x_m2, x_p1, x_p2, y_m1, y_m2, y_p1, y_p2, Tx, Ty, f, sol, sol_pred;
    
    do{
        convergence = 1;
        
        if (grid->rank == 0) {
            MPI_Isend(grid->grid+grid->X*grid->Y,(int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req);
        } else if (grid->rank == grid->size-1) {
            MPI_Isend(grid->grid+(2*grid->X), (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req+1);
        } else {
            MPI_Isend(grid->grid+grid->X*grid->Y, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req);
            MPI_Isend(grid->grid+(2*grid->X), (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req+1);
        }
        
        # pragma omp parallel for
        for (y = 0; y<grid->Y; y++) {
            for (x = 0; x<grid->X; x++) {
                if (grid->velocity[ (y+2)*grid->X + x ] < 0){
                    grid->grid[ (y+2)*grid->X + x] = INFTY;
                    continue;
                }
                
                x_m1   = (x >= 1) ? grid->grid[ (y+2)*grid->X + x-1] : INFTY;
                x_m2   = (x >= 2) ? grid->grid[ (y+2)*grid->X + x-2] : INFTY;
                x_p1   = (x+1 < grid->X) ? grid->grid[ (y+2)*grid->X + x+1] : INFTY;
                x_p2   = (x+2 < grid->X) ? grid->grid[ (y+2)*grid->X + x+2] : INFTY;
                y_m1   = grid->grid[ (y+1)*grid->X + x];
                y_m2   = grid->grid[ y*grid->X + x];
                y_p1   = grid->grid[ (y+3)*grid->X + x];
                y_p2   = grid->grid[ (y+4)*grid->X + x];
                f      = grid->velocity[ (y+2)*grid->X + x];
                sol_pred = grid->grid[ (y+2)*grid->X + x];
 
                if (x_p2 < INFTY && x_m2 < INFTY && y_m2 < INFTY && y_p2 < INFTY){
                    Tx = fminf( (4*x_m1-x_m2)/3.0, (4*x_p1-x_p2)/3.0 );
                    Ty = fminf( (4*y_m1-y_m2)/3.0, (4*y_p1-y_p2)/3.0 );
                    sol = solveEquation_2(Tx,Ty,f);
                }else{
                    Tx = fminf(x_m1, x_p1);
                    Ty = fminf(y_m1, y_p1);
                    sol = solveEquation_1(Tx,Ty,f);
                }
                
                grid->grid[ (y+2)*grid->X + x] = sol;
                convergence = convergence && (fabsf(sol - sol_pred) <= EPSILON);
                
            }
        }
        if (grid->rank == 0) {
            MPI_Irecv(grid->grid+grid->X*grid->Y, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req+1);
            MPI_Waitall(1, req, sta);
        } else if (grid->rank == grid->size-1) {
            MPI_Irecv(grid->grid, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req);
            MPI_Waitall(1, req, sta);
        } else {
            MPI_Irecv(grid->grid+grid->X*grid->Y, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank+1), tag, MPI_COMM_WORLD, req+1);
            MPI_Irecv(grid->grid, (int)(2*grid->X), MPI_FLOAT, (int)(grid->rank-1), tag, MPI_COMM_WORLD, req);
            MPI_Waitall(2, req, sta);
        }
        MPI_Allreduce(&convergence, &convergence_all, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        nb_iter++;
    } while (!convergence_all);
    
    Pnum *rcvbuf = NULL;
    if (grid->rank == 0) {
        rcvbuf = (Pnum*) malloc(grid->N*grid->N*sizeof(Pnum));
        MPI_Gather(grid->grid,(int)(grid->X*grid->Y), MPI_FLOAT, rcvbuf,(int)( grid->X*grid->Y), MPI_FLOAT, 0, MPI_COMM_WORLD);
        writeVTK(rcvbuf, grid->N, grid->file_name);
    }else{
        MPI_Gather(grid->grid,(int)(grid->X*grid->Y), MPI_FLOAT, rcvbuf,(int)( grid->X*grid->Y), MPI_FLOAT, 0, MPI_COMM_WORLD);
    }
}