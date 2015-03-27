//
//  velocity.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "velocity.h"

int inDisk(Pnum x, Pnum y, velocity_t * velocity){
    return (x-velocity->center_x)*(x-velocity->center_x)
    + (y-velocity->center_y)*(y-velocity->center_y)
    < velocity->radius*velocity->radius;
}

Pnum gaussian(Pnum x, Pnum y, Pnum x_0, Pnum y_0, Pnum sigma_x, Pnum sigma_y){
    return expf(-( (x-x_0)*(x-x_0)/(2*sigma_x*sigma_x) + (y-y_0)*(y-y_0)/(2*sigma_y*sigma_y) ));
}

void init_velocity_const(grid_t *grid, velocity_t *velocity){
    memset(grid->velocity, velocity->v, grid->X*grid->Y*sizeof(Pnum));
}

void init_velocity_gaussian(grid_t *grid, velocity_t *velocity){
    int x,y;
    for (y=0; y<grid->Y; y++)
        for (x=0; x<grid->X; x++)
            grid->velocity[(y+2)*grid->X + x] = gaussian(x,y,velocity->center_x,velocity->center_y,
                                                      velocity->sigma_x,velocity->sigma_y);
}

void init_velocity_random(grid_t *grid, velocity_t *velocity){
    int x,y;
    for (y=0; y<grid->Y; y++)
        for (x=0; x<grid->X; x++)
            grid->velocity[(y+2)*grid->X + x] = rand()/(Pnum)RAND_MAX;
}

void init_velocity_square(grid_t *grid,velocity_t *velocity){
    size_t x,y;
    
    size_t  coin_sup_x = velocity->length/2+velocity->center_x,
    coin_inf_x = velocity->center_x-velocity->length/2,
    coin_sup_y = velocity->length/2+velocity->center_y,
    coin_inf_y = velocity->center_y-velocity->length/2;
    
    if ( grid->rank*grid->N/grid->size <= coin_sup_y && coin_sup_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_sup_y %= grid->Y;
        
        for (y = 0; y < coin_sup_y ; y++) {
            for (x = coin_inf_x; x < coin_sup_x; x++) {
                grid->velocity[(y+2)*grid->X + x] = velocity->v;
            }
        }
    }
    
    if ( grid->rank*grid->N/grid->size <= coin_inf_y && coin_inf_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_inf_y %= grid->Y;
        
        for (y = coin_inf_y; y < grid->Y ; y++) {
            for (x = coin_inf_x; x < coin_sup_x; x++) {
                grid->velocity[(y+2)*grid->X + x] = velocity->v;
            }
        }
    }
    
    if (grid->rank*grid->N/grid->size > coin_inf_y && (grid->rank+1)*grid->N/grid->size -1 < coin_sup_y ) {
        /* Traverse le process */
        
        for (y=0; y<grid->Y; y++) {
            for (x = coin_inf_x; x < coin_sup_x; x++) {
                grid->velocity[(y+2)*grid->X + x] = velocity->v;
            }
        }
    }
}

void init_velocity_rectangle(grid_t *grid,velocity_t *velocity){

    size_t x,y;
    
    size_t  coin_sup_x = velocity->width/2+velocity->center_x,
    coin_inf_x = velocity->center_x-velocity->width/2,
    coin_sup_y = velocity->length/2+velocity->center_y,
    coin_inf_y = velocity->center_y-velocity->length/2;
    
    if ( grid->rank*grid->N/grid->size <= coin_sup_y && coin_sup_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_sup_y %= grid->Y;
        
        for (y = 0; y < coin_sup_y ; y++) {
            for (x = coin_inf_x; x < coin_sup_x; x++) {
                grid->velocity[(y+2)*grid->X + x] = velocity->v;
            }
        }
    }
    
    if ( grid->rank*grid->N/grid->size <= coin_inf_y && coin_inf_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_inf_y %= grid->Y;
        
        for (y = coin_inf_y; y < grid->Y ; y++) {
            for (x = coin_inf_x; x < coin_sup_x; x++) {
                grid->velocity[(y+2)*grid->X + x] = velocity->v;
            }
        }
    }
    
    if (grid->rank*grid->N/grid->size > coin_inf_y && (grid->rank+1)*grid->N/grid->size -1 < coin_sup_y ) {
        /* Traverse le process */
        
        for (y=0; y<grid->Y; y++) {
            for (x = coin_inf_x; x < coin_sup_x; x++) {
                grid->velocity[(y+2)*grid->X + x] = velocity->v;
            }
        }
    }
}

void init_velocity_circle(grid_t *grid, velocity_t *velocity){
    long int x, y, m;
    x = 0 ;
    y = velocity->radius;
    m = 5 - 4*velocity->radius ;
    while (x <= y) {
        
        if ( grid->rank*grid->N/grid->size <= y+velocity->center_y && y+velocity->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (y+velocity->center_y+2) * grid->X +  x+velocity->center_x ] = velocity->v;
            grid->grid[ (y+velocity->center_y+2) * grid->X -  x+velocity->center_x ] = velocity->v;
        }
        
        if ( grid->rank*grid->N/grid->size <= x+velocity->center_y && x+velocity->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (x+velocity->center_y+2) * grid->X + y+velocity->center_x ] = velocity->v;
            grid->grid[ (x+velocity->center_y+2) * grid->X - y+velocity->center_x ] = velocity->v;
        }
        
        if ( grid->rank*grid->N/grid->size <= -y+velocity->center_y && -y+velocity->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (-y+velocity->center_y+2) * grid->X + x+velocity->center_x ] = velocity->v;
            grid->grid[ (-y+velocity->center_y+2) * grid->X - x+velocity->center_x ] = velocity->v;
        }
        
        if ( grid->rank*grid->N/grid->size <= -x+velocity->center_y && -x+velocity->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (-x+velocity->center_y+2) * grid->X + y+velocity->center_x ] = velocity->v;
            grid->grid[ (-x+velocity->center_y+2) * grid->X - y+velocity->center_x ] = velocity->v;
        }
        
        if (m > 0) {
            y-- ;
            m -= 8*y ;
        }
        x++ ;
        m += 8*x+4 ;
    }
}

void init_velocity_disk(grid_t *grid, velocity_t *velocity){
    size_t x,y;
    init_velocity_circle(grid, velocity);
    for (y=0; y<grid->Y; y++) {
        for (x=0; x<grid->X; x++) {
            if (inDisk(x,y,velocity))
                grid->velocity[ (y+2) * grid->X + x] = velocity->v;
            }
        }
    }

void init_velocity(grid_t * grid, velocity_t * velo){
    init_velocity_const(grid, velo);
    switch (velo->fig) {
        case Circle:
            init_velocity_circle(grid, velo);
            break;
        case Square:
            init_velocity_square(grid, velo);
            break;
        case Rectangle:
            init_velocity_rectangle(grid, velo);
            break;
        case Gaussian:
            init_velocity_gaussian(grid, velo);
            break;
        case Disk:
            init_velocity_disk(grid, velo);
            break;
        case Random:
            init_velocity_random(grid, velo);
            break;
        default:
            break;
    }
}

