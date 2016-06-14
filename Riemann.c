#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define XINICIAL 0
#define XFINAL 10
#define PASOS 100
#define MASTER 0
#define TAG_X 1
#define TAG_RESULT_PARCIAL 2

float calcularFuncion(float x);

int main (int argc, char *argv[])
{
      int rank, value, size, dest,i,workers;  
      float resultado,incremento,x,resultParcial;

      MPI_Status status; 

      incremento = ((float)XFINAL-(float)XINICIAL)/(float)PASOS;

      MPI_Init( &argc, &argv );

      MPI_Comm_rank( MPI_COMM_WORLD, &rank );
      MPI_Comm_size( MPI_COMM_WORLD, &size ); 

      workers=size-1;

      if (size<2)
      {
            printf("No se puede procesar con %i nodos\n",size);
            return 0;
      }

      if (rank==MASTER)
      {
            dest=1;
            x=XINICIAL;
            for (i=0;i<PASOS;i++)
            {
                  MPI_Send(&x, 1, MPI_FLOAT, dest, TAG_X, MPI_COMM_WORLD); 
                  dest++;
                  if (dest==size) dest=1;
                  x+=incremento;
            }
            
            for (i=MASTER+1;i<size;i++)
            {
                  MPI_Recv(&resultParcial, 1, MPI_FLOAT, i, TAG_RESULT_PARCIAL, MPI_COMM_WORLD, &status); 
                  resultado +=resultParcial;
            }
      printf("El resultado de es %f \n",resultado);
            
      }
      
      else
      {
            for (i=0; i<PASOS-(((int)(PASOS/workers))*workers)>=rank? (int)(PASOS/workers)+1: (int)(PASOS/workers); i++) 
            {
            MPI_Recv(&x, 1, MPI_FLOAT, MASTER, TAG_X, MPI_COMM_WORLD, &status); 
            resultParcial += incremento * (calcularFuncion(x)+ calcularFuncion(x+incremento))/(float)2;
                  
            }
      MPI_Send(&resultParcial,1, MPI_FLOAT, MASTER, TAG_RESULT_PARCIAL, MPI_COMM_WORLD);
      }      
}

float calcularFuncion(float x) {  return x*x; }
