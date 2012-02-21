#include<omp.h> 

int main(int argc, char* argv[])
{
  int tid;
  int nthreads;
#pragma omp parallel private(tid)
  {
	tid = omp_get_thread_num();
	nthreads = omp_get_num_threads();
	printf("Thread num (rank) %d\n",tid);
#pragma omp master
   {
     printf("Number of threads %d\n",nthreads);
   }
  }
return 0;
}

