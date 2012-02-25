#include <mpi.h>
#include <stdio.h>

void main (int argc, char *argv[]) {
	
int myid, mysize, myid_left, myid_right, ierr;

MPI_Status istat;

MPI_Request ireq;

MPI_Init(&argc, &argv); /* Initialize MPI */

MPI_Comm_rank(MPI_COMM_WORLD, &myid); /* Get myid */

MPI_Comm_size(MPI_COMM_WORLD, &mysize); /*Get no. of processors*/



//printf("Processor %d of %d: Hello World!\n", myid, mysize);

/*compute myid_right and myid_left */



   myid_right = myid + 1;

if (myid_right >= mysize) { 
	myid_right = 0; 
	}

//myid_left = 0;

ierr = MPI_Isend(&myid, 1, MPI_INTEGER, myid_right, 0, MPI_COMM_WORLD, &ireq);

/* blocking receive from left neighbor */

ierr = MPI_Recv(&myid_left,1,MPI_INTEGER,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, &istat);

printf("Processor %d right of neighbor %d\n", myid, myid_left);

//modify this to also send all off these messages back - i.e. to the left neighbor processor.
//printf("Processor %d of %d: Hello World!\n", myid, mysize);

/*compute myid_right and myid_left */

myid_left = myid - 1;

if (myid_left == -1) { 
	myid_left = mysize-1; 
	}

//initialize myid_right - assigned by MPI Recv
myid_right = 0;

ierr = MPI_Isend(&myid, 1, MPI_INTEGER, myid_left, 0, MPI_COMM_WORLD, &ireq);

/* blocking receive from left neighbor */
ierr = MPI_Recv(&myid_right,1,MPI_INTEGER,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, &istat);

printf("Processor %d left of neighbor %d\n", myid, myid_right);

MPI_Finalize(); /* Terminate MPI */

}
