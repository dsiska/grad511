#include <stdio.h>
#include <mpi.h>

void main (int argc, char *argv[]){
int myId, mySize;
MPI_Init(&argc, &argv); //Initialize MPI
MPI_Comm_rank(MPI_COMM_WORLD, &myId); //get myID
MPI_Comm_size(MPI_COMM_WORLD, &mySize); //get # of processors
printf("Processor %d of %d: Hello World!\n", myId, mySize);

MPI_Finalize(); //terminate MPI

}
