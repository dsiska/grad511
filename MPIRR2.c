#include <stdio.h>
#include <MPI.h>

main (int argx, char* argv[]) {

	int procID, nproc, myID, myLeft, myRight, root, target, tag, source;
	myId = myLeft = myRight = root = target = 0;
	char string[20];

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
	target++;
	MPI_Send(&procID, 1, MPI_INT, target, tag, MPI_COMM_WORLD);
     } else {
	     	MPI_Recv(&myLeft, 1, MPI_INT, ANY, tag, MPI_COMM_WORLD);
	printf("I'm %d and %d just said hi to me!", procID, myLeft);	
	//MPI_Recv Blocking
	//MPI_Send
	//to nproc-1
     }
}
