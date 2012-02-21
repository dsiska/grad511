#include <stdio.h>
#include <omp.h>
void main (int argc, char *argv[])
{
int myid, mysize, m;
#pragma omp parallel private(myid) /* Initialize OpenMP */
{
myid = omp_get_thread_num(); /* Get myid */
mysize = omp_get_num_threads(); /*Get no. of processors*/
#pragma omp for
for (m = 1; m<=mysize; m++)
{ printf("Thread %d of %d total threads: Hello World!\n", myid,
mysize); }
}
}
