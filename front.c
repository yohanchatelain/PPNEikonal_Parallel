//
//  front.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "front.h"

void init_front_points(grid_t *grid, front_t * front){
    int i;
    for (i=0; i<2*front->nb_points; i+=2) {
        if ( grid->rank*grid->N/grid->size <= front->points[i+1] && front->points[i+1] <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (front->points[i+1]+2) * grid->X + front->points[i] ] = 0.0;
        }
    }
}

//Initialise un front source de forme carré
void init_front_square(grid_t *grid,front_t * front){
    size_t i;
    
    size_t  coin_sup_x = front->length/2+front->center_x,
    coin_inf_x = front->center_x-front->length/2,
    coin_sup_y = front->length/2+front->center_y,
    coin_inf_y = front->center_y-front->length/2;
    
    if ( grid->rank*grid->N/grid->size <= coin_sup_y && coin_sup_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_sup_y %= grid->Y;
        
        for (i = coin_inf_x; i < coin_sup_x; i++) {
            grid->grid[(coin_sup_y+2)*grid->X + i] = 0.0;
        }
        
        for (i = 0; i < coin_sup_y ; i++) {
            grid->grid[ (i+2)*grid->X + coin_inf_x ] = 0.0;
            grid->grid[ (i+2)*grid->X + coin_sup_x ] = 0.0;
        }
    }
    
    if ( grid->rank*grid->N/grid->size <= coin_inf_y && coin_inf_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_inf_y %= grid->Y;
        
        for (i = coin_inf_x; i < coin_sup_x; i++) {
            grid->grid[(coin_inf_y+2)*grid->X + i] = 0.0;
        }
        
        for (i = coin_inf_y; i < grid->Y ; i++) {
            grid->grid[ (i+2)*grid->X + coin_inf_x ] = 0.0;
            grid->grid[ (i+2)*grid->X + coin_sup_x ] = 0.0;
        }
    }
    
    if (grid->rank*grid->N/grid->size > coin_inf_y && (grid->rank+1)*grid->N/grid->size -1 < coin_sup_y ) {
        /* Traverse le process */
        
        for (i=0; i<grid->Y; i++) {
            grid->grid[ (i+2)*grid->X + coin_inf_x ] = 0.0;
            grid->grid[ (i+2)*grid->X + coin_sup_x ] = 0.0;
        }
    }
}

//Initialise un front source de forme circulaire
void init_front_circle(grid_t *grid, front_t *front){
    long int x, y, m;
    x = 0 ;
    y = front->radius;
    m = 5 - 4*front->radius ;
    while (x <= y) {
        
        if ( grid->rank*grid->N/grid->size <= y+front->center_y && y+front->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (y+front->center_y+2) * grid->X +  x+front->center_x ] = 0.0;
            grid->grid[ (y+front->center_y+2) * grid->X -  x+front->center_x ] = 0.0;
        }
        
        if ( grid->rank*grid->N/grid->size <= x+front->center_y && x+front->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (x+front->center_y+2) * grid->X + y+front->center_x ] = 0.0;
            grid->grid[ (x+front->center_y+2) * grid->X - y+front->center_x ] = 0.0;
        }
        
        if ( grid->rank*grid->N/grid->size <= -y+front->center_y && -y+front->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (-y+front->center_y+2) * grid->X + x+front->center_x ] = 0.0;
            grid->grid[ (-y+front->center_y+2) * grid->X - x+front->center_x ] = 0.0;
        }
        
        if ( grid->rank*grid->N/grid->size <= -x+front->center_y && -x+front->center_y <= (grid->rank+1)*grid->N/grid->size - 1) {
            grid->grid[ (-x+front->center_y+2) * grid->X + y+front->center_x ] = 0.0;
            grid->grid[ (-x+front->center_y+2) * grid->X - y+front->center_x ] = 0.0;
        }
        
        if (m > 0) {
            y-- ;
            m -= 8*y ;
        }
        x++ ;
        m += 8*x+4 ;
    }
}

//Initialise un front source linéaire
void init_front_segment(grid_t *grid, front_t *front){
    
    long int dx, dy;
    long int x1 = front->x1, x2 = front->x2,
    y1 = front->y1, y2 = front->y2;
    
    if ((dx = x2 - x1) != 0) {
        if (dx > 0) {
            if ((dy = y2 - y1) != 0) {
                if (dy > 0) {
                    if (dx >= dy) {
                        long int e ;
                        dx = (e = dx) * 2 ; dy *= 2;
                        while(1){
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (x1++ == x2) break ;
                            if ((e -= dy) < 0) {
                                y1++;
                                e += dx ;
                            }
                        }
                    }else{
                        long int e ;
                        dy = (e = dy) * 2 ; dx *= 2;
                        while(1){
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (y1++ == y2) break ;
                            if ((e -= dx) < 0){
                                x1++;
                                e += dy ;
                            }
                        }
                    }
                }else{
                    if (dx >= -dy) {
                        long int e ;
                        dx = (e = dx) * 2 ; dy *= 2;
                        while(1) {
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (x1++ == x2) break ;
                            if ((e += dy) < 0) {
                                y1--;
                                e += dx ;
                            }
                        }
                    }else{
                        long int e ;
                        dy = (e = dy) * 2 ; dx *= 2;
                        while(1) {
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (y1-- == y2) break ;
                            if ((e += dx) > 0) {
                                x1++;
                                e += dy ;
                            }
                        }
                    }
                    
                }
            }else{
                do{
                    if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                        grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                    }
                }while((x1++) != x2) ;
                
            }
        }else{
            if ((dy = y2 - y1) != 0) {
                if (dy > 0) {
                    if (-dx >= dy) {
                        long int e ;
                        dx = (e = dx) * 2 ; dy *= 2;
                        while(1) {
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (x1-- == x2) break ;
                            if ((e += dy) >= 0) {
                                y1++;
                                e += dx ;
                            }
                        }
                    }else{
                        long int e ;
                        dy = (e = dy) * 2 ; dx *= 2;
                        while(1) {
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (y1++ == y2) break;
                            if ((e += dx) <= 0) {
                                x1--;
                                e += dy ;
                            }
                        }
                    }
                    
                } else {
                    if (dx <= dy) {
                        long int e ;
                        dx = (e = dx) * 2 ; dy *= 2;
                        while(1) {
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (x1-- == x2) break ;
                            if ((e -= dy) >= 0) {
                                y1--;
                                e += dx ;
                            }
                        }
                    } else {
                        long int e ;
                        dy = (e = dy) * 2 ; dx *= 2;
                        while(1) {
                            if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                                grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                            }
                            if (y1-- == y2) break ;
                            if ((e -= dx) >= 0) {
                                x1--;
                                e += dy ;
                            }
                        }
                    }
                    
                }
            } else {
                do{
                    if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                        grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                    }
                }while((x1--) != x2);
                
            }
        }
    } else {
        if ((dy = y2 - y1) != 0) {
            if (dy > 0) {
                do{
                    if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                        grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                    }
                }while((y1 = y1 + 1) != y2);
            } else {
                do{
                    if ( grid->rank*grid->N/grid->size <= y1 && y1 <= (grid->rank+1)*grid->N/grid->size - 1) {
                        grid->grid [ (y1+2) * grid->X + x1 ] = 0.0;
                    }
                }while((y1--) != y2);
                
            }
        }
    }
}

//Initialise un front source rectangulaire
void init_front_rectangle(grid_t * grid, front_t *front){
    size_t i;
    
    size_t  coin_sup_x = front->width/2+front->center_x,
    coin_inf_x = front->center_x-front->width/2,
    coin_sup_y = front->length/2+front->center_y,
    coin_inf_y = front->center_y-front->length/2;
    
    if ( grid->rank*grid->N/grid->size <= coin_sup_y && coin_sup_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_sup_y %= grid->Y;
        
        for (i = coin_inf_x; i < coin_sup_x; i++) {
            grid->grid[(coin_sup_y+2)*grid->X + i] = 0.0;
        }
        
        for (i = 0; i < coin_sup_y ; i++) {
            grid->grid[ (i+2)*grid->X + coin_inf_x ] = 0.0;
            grid->grid[ (i+2)*grid->X + coin_sup_x ] = 0.0;
        }
    }
    
    if ( grid->rank*grid->N/grid->size <= coin_inf_y && coin_inf_y <= (grid->rank+1)*grid->N/grid->size - 1) {
        /* Démarre dans le process */
        coin_inf_y %= grid->Y;
        
        for (i = coin_inf_x; i < coin_sup_x; i++) {
            grid->grid[(coin_inf_y+2)*grid->X + i] = 0.0;
        }
        
        for (i = coin_inf_y; i < grid->Y ; i++) {
            grid->grid[ (i+2)*grid->X + coin_inf_x ] = 0.0;
            grid->grid[ (i+2)*grid->X + coin_sup_x ] = 0.0;
        }
    }
    
    if (grid->rank*grid->N/grid->size > coin_inf_y && (grid->rank+1)*grid->N/grid->size -1 < coin_sup_y ) {
        /* Traverse le process */
        
        for (i=0; i<grid->Y; i++) {
            grid->grid[ (i+2)*grid->X + coin_inf_x ] = 0.0;
            grid->grid[ (i+2)*grid->X + coin_sup_x ] = 0.0;
        }
    }
}

void init_front(grid_t * grid, front_t * front){
    switch (front->fig) {
        case Points:
            init_front_points(grid, front);
            break;
        case Segment:
            init_front_segment(grid, front);
            break;
        case Circle:
            init_front_circle(grid, front);
            break;
        case Square:
            init_front_square(grid, front);
            break;
        case Rectangle:
            init_front_rectangle(grid, front);
            break;
        default:
            break;
    }
}


