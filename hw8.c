//
// program to perform Successive Over-Relaxation
//   on the unit square
//
// Define variables
//  i      = row number
//  j      = column number
//  k      = iteration count
//  kmax   = maximum number of iterations
//  l      = power of 2 counter
//  m      = size of problems (no. of intervals)
//  mm1    = m - 1
//  omega  = relaxation parameter
//           > 1, over-relaxation
//           < 1, under-relaxation
//  res    = residual at point i, j
//  resmax = maximum residual at iteration k
//  rest   = residual array of t array
//  t      = temperature array
//  tnew   = initial "new" temperature
//  tol    = tolerance for maximum residual
//  told   = "old" temperature
//
//
// set storage, define variables
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FALSE 0
#define TRUE 1
#define m 128

int main(){

  //vector<vector<double> >  t, Rest;
  int temp_size = (m+1)*(m+1);
  double * t = (double *) malloc(temp_size*sizeof(double));
  memset(t,0,temp_size);
double errorc;
double omega, res, resmax, tnew, tol, told;
 int i;
 int  isave;
 int j;
 int jsave;
 int k;
 int kmax;
 int  mm1;

//
// set size of problem
//

kmax = 1000000;
omega = 1.4;
tol = 0.0000001;

//
// get index inside bounmdary
//
mm1 = m - 1;
//
// initialize matrix
//

//
// initialize top row, t = 1
//
for(j = 1; j <= mm1; ++j){
  t[j + m*m] = 1;
  //t[m + j*m] = 1;
}
//
// perform iterations until the error
//  is < tol, or until kmax is exceeded
//
k=1;
resmax = 1.e30;
int resmax_bool=FALSE;
int kmax_bool=TRUE;

while(k<kmax && !resmax_bool){
  resmax = -1.e30;
  for(i = 1;i <= mm1;++i){
    for(j = 1; j <= mm1;++j){
      told = t[m*i + j];
      tnew = 0.25*(t[(i+1)*m + j] + t[(i-1)*m + j] + t[i*m + (j+1)] + t[i*m + (j-1)]);
      t[m*i + j] = told + omega*(tnew - told);
      res = fabs(tnew-told);
      if(res > resmax){
        resmax = res;
        isave = i;
        jsave = j;
	}
    }
 } 
  if(resmax < tol){
	resmax_bool = TRUE;
	kmax_bool = FALSE;
  }
++k;
}
//
if(kmax_bool){
	printf("Reached maximum iterations kmax = %d without converging\n",kmax);
}
if(resmax_bool){
	printf("Converged at no. of iterations k = %d\n",k);
}
printf("maximum residual = %.16e at i, j = %d , %d \n",resmax,isave,jsave);
//
// compute error in centerpoint temperature
//
int middle = m/2;
errorc = 0.25-t[middle + m*middle];
printf("Centerpoint temperature = %.16e ; error = %.16e \n",t[middle + m*middle],errorc);
/*
for(j=0;j<6;++j){
 printf("%d, row: ",middle+j-3);
 for(i=0;i<6;++i){
   printf("%f ,",t[(middle-3+j) + (m*middle-3+i)]);
 }
 printf("\n");
}
*/
printf("  for problem size, m = %d \n", m);
//
// should print t(i,j) here
// should also compute and print residual matrix here
//
//
};

