#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>


#define isize  10000000
//#define isize  100

void readfile(char * inputfile, long  * retVal ){

    char line[30];
 
    //allocate first set of data for vector.
    FILE * file = fopen(inputfile,"r");
    long tempNum;
    long i=0;
    while(fgets(line,80,file) != NULL)
    {
      sscanf(line,"%d",&tempNum);//convert to double
      retVal[i] = tempNum;
      ++i;
    }
printf("%d integer values read successfully\n",i); 
}

int evaluate(long a, long b, int isort){
int temp;
if(isort==0){
	if(a < b)
	   temp=1;
	else
	   temp=0;
}else{
	if(a > b)
	   temp=1;
	else
	   temp=0;
}
return temp;	
}

void siftDown(long * numbers, long root, long bottom,int isort)
{
  long done, maxChild, temp;
 
  done = 0;
  while ((root*2 <= bottom) && (!done))
  {
    int temp0 = evaluate(numbers[root * 2+1],numbers[root *2],isort); 
    if (root*2 == bottom){
      maxChild = root * 2;
	}
//    else if (numbers[root * 2] > numbers[root * 2 + 1]){
    else if (temp0==1){
      maxChild = root * 2;
    }else{
      maxChild = root * 2 + 1;
	};
    int result = evaluate(numbers[root],numbers[maxChild],isort); 
    if (result==1)
    {
      temp = numbers[root];
      numbers[root] = numbers[maxChild];
      numbers[maxChild] = temp;
      root = maxChild;
    }
    else
      done = 1;
  }
}

void heapsort(long * numbers, long n, int isort)
{
  long i, temp;
 
  for (i = (n / 2); i >= 0; i--)
    siftDown(numbers, i, n - 1,isort);
 
  for (i = n-1; i >= 1; i--)
  {
    temp = numbers[0];
    numbers[0] = numbers[i];
    numbers[i] = temp;
    siftDown(numbers, 0, i-1,isort);
  }
}
 
 

//**********************************************************************
// program to perform a parallel sort
//**********************************************************************
//
// ia[n]     = real array to be sorted by index
// index[n]  = sorted indices of array elements
// n    	    = counter
// ntot      = total number of data elements to be sorted
// isort     = 0, ascending sort; 1, descending sort
// iprlong    = 0, do not prlong; 1, prlong beginning, middle and end values
//

int main(int argc, char ** argv){
 long ierr, n, ntot;
 int myid, mysize;
 long icheck,isort,iprint;
 double t1, t2, tdiff;
 long * ia = (long *) malloc(isize*sizeof(long));
 long * index = (long *) malloc(isize*sizeof(long)); 
 memset(ia,0,isize);
 memset(index,0,isize);

//**********************************************************************
//  Initialization
//**********************************************************************
t1 = omp_get_wtime();
isort = 0;
iprint = 1;
ntot = isize;
//**********************************************************************
// Initialize MPI and Get myrank:
//**********************************************************************
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&mysize);
//**********************************************************************
//  Get mysize:
//**********************************************************************

MPI_Comm_rank(MPI_COMM_WORLD,&myid);
//**********************************************************************
//  worker task
//**********************************************************************
long elements_to_print[30];
if (myid == 0){
  printf("myid = %d of total size = %d\n",myid,mysize);

//**********************************************************************
// open file and read ia array
//***********************************************************************/
  printf("reading input from file hw7.in\n");
  readfile("hw7.in",ia);
// =================== IMPORT THE FILE hw7.in ==========================


  printf("File opened successfully. Reading input values\n");
//  read (1,*,err = 30) ia
//  prlong *,isize,' longeger values read successfully'
//  go to 40
//
//  30 prlong *,'Error reading input values --- Stopping!'
//  go to 999

//**********************************************************************
// calling heapsort
//**********************************************************************
  printf("myid = %d; calling heapsort\n",myid);
  heapsort(ia, ntot, isort);

/***********************************************************************
// check that data are in ascending order
***********************************************************************/
  icheck = 0;
  if(isort == 0){
//=================== ascending ========================================
    for(n = 1; n < ntot;++n){
 
      if (ia[n] < ia[n-1]){
        printf("Error in sorted order at n = %d\n",n);
	icheck = 1;
      };
    };
  }else{
//==================== descending ======================================
    for(n = 1; n < ntot;++n){
      if (ia[n] > ia[n]){
        printf("Error in sorted order at n = %d \n",n);
	icheck = 1;
      };
    };
  };

/***********************************************************************
// termination test
***********************************************************************/
  if(icheck == 0){
     printf("Array sorted correctly\n");
  }else{
     printf("Array sorted incorrectly\n");
  };
/***********************************************************************
//  prlong sorted indices
***********************************************************************/

 long element_count=0;
  if(iprint == 1){
/***********************************************************************
//  prlong 1st 10 elements
***********************************************************************/
    for(n = 0; n< 10;++n){
      //elements_to_prlong.push_back(ia[index[n]]);
      elements_to_print[element_count] = ia[n];
      ++element_count;
    };
/***********************************************************************
//  prlong middle 10 elements
***********************************************************************/
    for(n = ((ntot/2)-5); n< ((ntot/2)+5);++n){
      //elements_to_prlong.push_back(ia[index[n]]);
      elements_to_print[element_count] = ia[n];
      ++element_count;
    }
/***********************************************************************
//  prlong last 10 elements
***********************************************************************/
    for(n = (ntot-10); n < ntot;++n){
      //elements_to_prlong.push_back(ia[index[n]]);
      elements_to_print[element_count] = ia[n];
      ++element_count;
    }; 
 }; 
/*****************************************************
 get total time
******************************************************/
t2 = omp_get_wtime();
tdiff = t2 - t1;
printf("\tPrintng 1st 10 elements\n");
 long j;
 for(j=0;j<10;++j){
   printf("\t%d, \t%d\n",j+1,elements_to_print[j]);
 }
 printf("\tPrintng middle 10 elements\n");
  for(j=10;j<20;++j){
    printf("\t%d, \t%d\n",j+(ntot)/2-15+1,elements_to_print[j]);
 }
 printf("\tPrintng last 10 elements\n");
 for(j=20;j<30;++j){
   printf("\t%d, \t%d\n",j+ntot-30+1,elements_to_print[j]);
 }
printf("Total wall clock time = %f  (secs) --- Stopping!\n",tdiff);
}else{
//**********************************************************************
//**********************************************************************
//  worker tasks
//**********************************************************************
//**********************************************************************
  printf("myid = %d of total size = %d\n",myid,mysize);
} 

 
//
// terminate MPI
//
MPI_Finalize();
//
//3000 format (1x,'Calling hsort: isort,iprint =',2(1x,i5))
//3001 format (5x,'Before sort: printing ia with',i12,' elements')
//3002 format (5x,'Final: printing ia with',i12,' elements')
//3003 format (5x,'Printing 1st 10 elements')
//3004 format (5x,'Printing middle 10 elements')
//3005 format (5x,'Printing last 10 elements')
};
