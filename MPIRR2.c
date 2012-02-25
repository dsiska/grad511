#include <stdio.h>
#include <mpi.h>

main (int argc, char* argv[]) {

	int procID, nproc, myId, myLeft, myRight, root, target, tag, source;
	myId = myLeft = myRight = root = target = 0;
	char string[20];
	MPI_Status status;

	// Initialize MPI
     MPI_Init(&argc, &argv);

     // Get process rank
     MPI_Comm_rank(MPI_COMM_WORLD, &procID);

     // Get total number of processes specificed at start of run
     MPI_Comm_size(MPI_COMM_WORLD, &nproc);


// 0 send int - I am procID, hello!
// 1 receive int
// 1 send int - oh well int, I am procID!
// 0 receive int
// 0 print - well golly it sure is nice to meet you int!
tag = procID;

     if (procID == root) {
	target = 1;
	MPI_Send(&procID, 0, MPI_INT, target, tag, MPI_COMM_WORLD);
     }
     
     if (procID != root) {
	     	MPI_Recv(&myLeft, 1, MPI_INT,MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
	printf("I'm %d and %d just said hi to me!", procID, myLeft);	
	//MPI_Recv Blocking
	//MPI_Send
	//to nproc-1
     }
}
