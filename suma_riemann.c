/**
* FILE: riemann_erroneo.c
* DESCRIPTION:
*   	Implementacion de la suma de Riemann para calcular el area bajo la
*   	curva.
* AUTHOR: John Sanabria - john.sanabria@correounivalle.edu.co
* LAST REVISED: 14/04/16
* REFERENCES: http://mathinsight.org/calculating_area_under_curve_riemann_sums
******************************************************************************/

#include <stdio.h>
#include "mpi.h"
#include <pthread.h>
#include <math.h>

#define MAXRECT 100
#define BASE 0.0
#define LIMIT 10.0
//#define MAXTHREAD 10

double length = LIMIT - BASE;
double base_length = 0;
//double numRectxThread = (double)(MAXRECT/MAXTHREAD);
//double sumTotal = 0;
//double partialSums[MAXTHREAD];

double function(double x) 
{
    	return x * x;
}

/*
void* calcular(void *arg) {
    	long id = (long) arg;
    	double lowlimit = id*base_length*numRectxThread;

    	double sum = 0;

    	for (int i = 0; i < numRectxThread; i++) {
            	sum += function(lowlimit + i*base_length) * base_length;
    	}
    	partialSums[id] = sum;
    	printf("sum: %lf\n", sum);
    	return 0;
}
*/

int main(int argc, char** argv) {
    	
		
    	base_length = length/MAXRECT;
		
    	int numtasks, rank, source=0, dest, tag=1, i;
        
		MPI_Status stat;
        MPI_Datatype rowtype;
        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
		
		//pthread_t threads[MAXTHREAD];
    	//long taskids[MAXTHREAD];

        int taskids[numtasks], taman [numtasks];
        float lowlimit [numtasks], riem[1];
        double numRectxThread = (double)(MAXRECT/numtasks);
      		
		
		/*printf("base length: %lf numRectxThread: %lf\n",base_length, numRectxThread);
		
    	for (int i = 0; i < MAXTHREAD; i++) {
            	taskids[i] = i;
            	partialSums[i] = 0;
            	pthread_create(&threads[i], NULL, calcular, (void*)taskids[i]);
    	}
    	for (int i = 0; i < MAXTHREAD; i++) {
            	pthread_join(threads[i], NULL);
            	sumTotal += partialSums[i];
    	}
    	printf("Suma total %lf\n",sumTotal);
    	pthread_exit(NULL);*/		
		
		
		for ( i= 0; i < numtasks; i++) 
		{
				taman[i]=1;
                taskids[i] = i;
                lowlimit[i]= i*base_length*numRectxThread;
        }
        
        MPI_Scatterv(lowlimit,taman,taskids,MPI_FLOAT,riem,1,MPI_FLOAT,0,MPI_COMM_WORLD);	
        
		double otro = riem[0];
        float sum = 0;
		int j; 	
        
		for (j = 0; j < numRectxThread; j++) 
		{
            sum += function(otro + j*base_length) * base_length;
        }
		
        printf("Subtotal %f en nodo %d \n", sum, rank);
        
		MPI_Reduce(&sum,riem,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
        
		if (rank == 0) printf("TOTAL: %lf\n",riem[0]);
        
		MPI_Finalize();
		
}
