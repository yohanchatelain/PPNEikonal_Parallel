//
//  vtk_io.c
//  PPNEikonal_Parallel
//
//  Created by Yohan on 20/03/2015.
//  Copyright (c) 2015 Yohan. All rights reserved.
//

#include "vtk_io.h"

void writeVTK(Pnum *grid ,size_t N, char *file_name_matrix){
    

    size_t x,y;
    Pnum f;
    FILE* fp;
    
    char* vtk ="# vtk DataFile Version 3.0";
    char* type="Propagation de front";
    char* format = "ASCII";
    char* dataset="DATASET RECTILINEAR_GRID";
    fp = fopen(file_name_matrix,"w+");
    
    //Header
    printf("\n Writting data ....\n ");
    fprintf(fp,"%s\n%s\n%s\n%s\n",vtk,type,format,dataset);
    fprintf(fp,"DIMENSIONS %zu %zu %d\n",N,N,1);
    
    //Coordonnées x
    fprintf(fp,"X_COORDINATES %zu float\n",N);
    for(x=0;x<N;x++)
        fprintf(fp,"%f ",x*1.0);
    
    //Coordonnées y
    fprintf(fp,"\nY_COORDINATES %zu float\n",N);
    for(y=0;y<N;y++)
        fprintf(fp,"%f ",y*1.0);
    
    //Coordonnées z
    fprintf(fp,"\nZ_COORDINATES 1 float\n");
    fprintf(fp,"0.0");
    
    //Valeurs
    fprintf(fp,"\nPOINT_DATA %lu\n",N*N);
    fprintf(fp,"SCALARS u float 1\n");
    fprintf(fp,"LOOKUP_TABLE default\n");
    
    //Ecriture des valeurs
    for(y=0;y<N;y++)
        for(x=0;x<N;x++){
            f = grid[y*N + x];
            f = (f >= INFTY) ? -1 : f;
            fprintf(fp,"%f ",f);
        }
    printf("%s has been written\n",file_name_matrix);
    
    fclose(fp);
    
}
