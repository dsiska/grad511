#include <stdio.h>
#include <mpi.h>

main (int argc, char* argv[]) {


	int procID, nproc, myId, myLeft, myRight, root, target, tag, source, ierr, i;
	myId = myLeft = myRight = root = target = 0;
	char string[20];
	MPI_Status status;
	MPI_Request ireq;

	// Initialize MPI
     ierr = MPI_Init(&argc, &argv);

     // Get process rank
     ierr = MPI_Comm_rank(MPI_COMM_WORLD, &procID);

     // Get total number of processes specificed at start of run
     ierr = MPI_Comm_size(MPI_COMM_WORLD, &nproc);


// 0 send int - I am procID, hello!
// 1 receive int
// 1 send int - oh well int, I am procID!
// 0 receive int
// 0 print - well golly it sure is nice to meet you int!
tag = 1;

     if (procID == root) {
	printf("Root process initiating introductions");
	for (i=0; i < nproc; i++)
	ierr = MPI_Send(&procID, 0, MPI_INT, i, tag, MPI_COMM_WORLD);
     }
     else
     {
	MPI_Recv(&myLeft, 1, MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	
	//ierr = MPI_Recv(&myLeft, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
	printf("I'm %d and %d just said hi to me!\n", procID, myLeft);	
	
	//MPI_Recv Blocking
	//MPI_Send
	//to nproc-1
     }
ierr = MPI_Finalize();
}
