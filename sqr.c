#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

main(int argc, char** argv)
{
  double tbegin, tend, tdiff, sum, omp_get_wtime();
  long i, N;
  N = 1000000000;

  tbegin=omp_get_wtime();
  //compute sum of square roots
  sum = 0.;
  for (i = 1; i <= N; ++i)
    {
     	sum += sqrt(i);
    }
   tend=omp_get_wtime();
    
  tdiff =  (tend-tbegin);
  printf("Time: %.4lf s\n", tdiff);
  printf("sum: %f\n", sum);
}







