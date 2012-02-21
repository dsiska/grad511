#include <stdio.h>
#include <math.h> 

int main(argc,argv) 
int argc; 
char *argv[]; 
{ 

int done = 0, n, i, rc; 

double PI25DT = 3.141592653589793238462643; 
double pi, h, sum, x, stride=1;

while (!done){ 
    printf("Enter the number of intervals: (0 quits) "); 
    fflush(stdout);
    scanf("%d",&n); 
    if (n == 0) break; 

    sum = 0.0; 
    for (i = 0; i <= n; i += stride) { 
        sum += pow(-1,i)/(2*i+1);
    } 
    pi = 4 * sum;
    printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT)); 

} 

}

