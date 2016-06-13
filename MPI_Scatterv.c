#include "mpi.h"
#include <stdio.h>
#define SIZE 100

main(int argc, char *argv[])  {
        int numtasks, rank, source=0, dest, tag=1, i;
        int b[SIZE], acum, disp[SIZE], scounts[SIZE], a[SIZE];
        
        for (i = 1; i <= SIZE; i++)  a[ i - 1 ] = i;
        
        MPI_Status stat;
        MPI_Datatype rowtype;
        
        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        
        
        for (i = 0; i < numtasks; i++) 
        {
                disp[i] = i*(SIZE/numtasks);
                scounts[i] =(SIZE/numtasks);
        }

        MPI_Scatterv(a,scounts,disp,MPI_INT,b,SIZE/numtasks,MPI_INT,0,MPI_COMM_WORLD);
        
        acum = 0;
        for (i = 0; i < SIZE/numtasks; i++)  acum += b[i];
                
        printf("Subtotal %d en nodo %d\n", acum, rank);
        
        MPI_Reduce(&acum,b,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        
        if (rank == 0) printf("TOTAL: %d\n",b[0]);
        
        MPI_Finalize();
}
