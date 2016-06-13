#include <mpi.h>
#include <stdio.h>
#include <malloc.h>
#define MAX 100

int main(int argc, char** argv) {
    	
    int rank, size, i, acum, numtasks, rank;
    int b[MAX], vector[MAX], disp[MAX], scounts[MAX];
    
    for (i = 1; i <= 100; i++)
    	vector[ i - 1 ] = i;
    	
    MPI_Status stat;
    MPI_Datatype rowtype;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    for (i = 0; i < numtasks; i++) 
    {
      disp[i] = i*MAX;
      scounts[i] = MAX;
    }
   
    MPI_Scatterv(vector,scounts,disp,MPI_INT,b,MAX,MPI_INT,0,MPI_COMM_WORLD);
    
    acum = 0.0;
    for (i = 0; i < MAX; i++)
    	acum += b[i];		
				
    printf("Subtotal %lf en nodo %d\n", acum, rank);
    MPI_Reduce(&acum,b,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if (rank == 0) printf("TOTAL: %lf\n",b[0]);
   
    MPI_Finalize();
}
