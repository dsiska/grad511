#include <stdio.h>
#include <time.h>

#define  M 3000
#define  N 3000
#define  K 3000
#define  DEBUG

//C <- alpha*op(A)*op(B) + beta*C
void dgemm_(char *transa, char *transb, int *m, int *n, int *k, double *alpha,
           double *a, int *lda, double *b, int *ldb, double *beta, double *c,
           int *ldc, long lenta, long lentb);

void echo(char *arg);

double MPI_Wtime();
void MPI_Init();
void MPI_Finalize();

void main(int argc, int **argv) {
   char   transa = 'N'; //this means do not take the transverse
   char   transb = 'N'; //this means do not take the transverse
   int    m = M;
   int    n = N;
   int    k = K;
   int    lda = M; //leading dimension of a
   int    ldb = K; //leading dimension of b
   int    ldc = M; //leading dimension of c
   double alpha = 1.; //alpha * a * b
   double beta  = 0.; //beta * c
   long   len = 1;

   MPI_Init (argc, argv);

   /* Arrays in row major format
   A =    3    0   3   1   B = 1   2   C = 1  1
          1    4   0   2       3   1       1  1
          2    5   4   1       4   0       1  1
                               0   2
   double a[M*K] = {3., 1., 2., 0., 4., 5.,
                    3., 0., 4., 1., 2., 1.};
   double b[K*N] = {1., 3., 4., 0., 2., 1.,
                    0., 2.};
   double c[M*N] = {0., 1., 1., 1., 1., 1.};

   */
   double a[M*K] ;
   double b[K*N];
   double c[M*N];

   echo ("reading files\n");
   FILE *ain = fopen("../../../../infiles/a.in","r");

   int v ;
   int i = 0 ;
   for (i = 0; i < M*K; i++){
      fscanf(ain,"  %lf",&a[i]);
   }
  
   fclose(ain);

   FILE *bin = fopen("../../../../infiles/b.in","r");

   i = 0 ;
   for (i = 0; i < K*N; i++){
      fscanf(bin,"  %lf",&b[i]);
   }

   fclose (bin);

   double t0 = MPI_Wtime();
   double t1 = MPI_Wtime();
   #pragma omp parallel
   {
   dgemm_(&transa, &transb, &m, &n, &k, &alpha, a, &lda, b, &ldb,
          &beta, c, &ldc, len, len);
   }
   double t2 = MPI_Wtime();

   double t = ((double) (t2- t1 - (t1-t0))) ;
   double mflops = (N*M*K / t) / 1000000.0;
   printf ("\n\nruntime=%f secs\nmflops=%f\n",t,mflops);
   FILE *out = fopen ("c.out", "w");


   for (int i=0; i < M*N; i ++) {
       fprintf (out, "%f  ",c[i]);
   }

   fclose (out);
   MPI_Finalize();
}

void echo (char *arg) {
   #ifdef DEBUG 
      printf (arg); 
   #endif DEBUG
}
