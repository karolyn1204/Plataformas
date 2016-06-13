#include <mpi.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>

#define MAX 100

int main(int argc, char** argv) {

        float tt,inicio,final;
        inicio=clock();

        int rank, size, i, data, dest, source;
        int vector[MAX], *miparte, stride;
        int tag = 1,  acum = 0,  total = 0;
        
        MPI_Status stat;

        for (i = 1; i <= 100; i++)
                vector[ i - 1 ] = i;
        
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
        stride = MAX/(size - 1);
        
        if (rank == 0) 
        {
                for (i = 1; i < size; i++)
                        MPI_Send(&vector[stride*(i - 1)],stride,MPI_INT, i, tag, MPI_COMM_WORLD);
        } else 
        {
                acum = 0;
                miparte = (int*)malloc(sizeof(int)*stride);
                MPI_Recv(miparte, stride, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
                for (i = 0, acum = 0; i < stride; i++)
                        acum += miparte[i];
        }
        
        MPI_Reduce(&acum,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        
        if (rank == 0)
        {
                printf("Total: %d\n", total);
                final= clock();
                tt= (final-inicio)/(double) CLOCKS_PER_SEC;
                printf("Tiempo transcurrido implementacion a pie: %f\n\n", tt);
        }

        MPI_Finalize();
}
