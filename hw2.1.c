#include <time.h> //link with -lrt for the clock_gettime function.
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BILLION  1000000000L
#define MAXLINES 9000000
#define DEFAULTVECTORSIZE 20000
#define STRIDES 7
// uncomment this define if you are on a linux system and want to have faster IO speeds.
// if you are not on a linux system leave this define commented out and use the cross
// platform version.
// #define LINUX //use the fast version

typedef struct
{
    uint32_t size; //number of elements
    uint32_t allocSize; //size of allocated data
    double * data;
}vector;

double time_heuristics(struct timespec * start,struct timespec * stop){
    double accum = ( stop->tv_sec - start->tv_sec ) + (double)( stop->tv_nsec - start->tv_nsec ) / (double)BILLION;
    return(accum);
}

#ifndef LINUX
//this is a cross platform version that is not as fast as the linux specific.
vector * readfile(char * inputfile ){

    char lineBuf[30];
    int i;
    vector * retVal = (vector *) malloc(sizeof(vector));
    memset(retVal,0,sizeof(vector));
    double * array = (double *) calloc(DEFAULTVECTORSIZE,sizeof(double));
    int size = DEFAULTVECTORSIZE;

    //allocate first set of data for vector.
    FILE * file = fopen(inputfile,"r");
    double tempNum;
    for(i=0;fgets(lineBuf,29,file)!=NULL;++i)
    {
        tempNum = strtod(lineBuf,NULL);//convert to double
        //strtod is much faster than sscanf("%le") to convert to double
        if(i >= size)//need to increase the size of the list.
        {
            size = size*2;
            if(size > MAXLINES) size = MAXLINES;
            array= (double *) realloc(array,size*sizeof(double));
            if(array== NULL)
            {
                perror("realloc");
                exit(1);
            }
        }
        array[i]=tempNum;//does this work now?
    }
    fclose(file);
    retVal->data = array;
    retVal->size = i;
    retVal->allocSize = size;
    return retVal;
}

#else
//linux specific version that is very fast for large files.
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

vector * readfile(char * inputfile ){
    int fd = open(inputfile,O_RDONLY),i=0,size=DEFAULTVECTORSIZE;
    char *p,*nptr,*endptr;
    struct stat sb;
    vector * retval;
    double * data;
    memset(&sb,0,sizeof(struct stat));
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    if(fstat(fd,&sb)==-1)
    {
        perror("fstat");
        exit(1);
    }
    
    if(!S_ISREG(sb.st_mode))
    {
        fprintf(stderr,"%s is not a file.\n",inputfile);
        exit(1);
    }
    
    p= mmap(0,sb.st_size,PROT_READ, MAP_SHARED,fd,0);

    if(p == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    
    if(close(fd)== -1)
    {
        perror("close");
        exit(1);
    }
    //file area is not read to read from.
    retval = (vector *) malloc(sizeof(vector));
    memset(retval,0,sizeof(vector));
    data = (double *) calloc(DEFAULTVECTORSIZE,sizeof(double));
    nptr=p;
    do
    {
        if(i>=size)
        {
            size*=2;
            data=(double *)realloc(data,size*sizeof(double));
        }
        data[i]=strtod(nptr,&endptr);
        if(endptr==nptr)break;//hit the end of the data
        
        nptr=endptr;
        i++;
    }while(1);
    
    if(munmap(p,sb.st_size)== -1)
    {
        perror("munmap");
        free(retval);
        exit(1);
    }

    retval->size=i;
    retval->data=data;
    retval->allocSize=size;
    return retval;
}
#endif

double dot_product_stride(int N, vector* a_in,vector * b_in,int stride){
  double dot_product = 0;
  int i;
  for(i=0;i<N;i+=stride){
    dot_product += a_in->data[i]*b_in->data[i];
  }
  return dot_product;
}


int main(void){
  int N = 100;


  struct timespec start,stop;
  double deltat32,deltat54;
  
  //Reading data file "a.in"
  printf(" Reading a\n");
  clock_gettime(CLOCK_MONOTONIC,&start);
  vector *a_in = readfile("a.in");
  //Reading data file "b.in"
  printf(" Reading b\n");
  vector *b_in = readfile("b.in");
 clock_gettime(CLOCK_MONOTONIC,&stop);
 deltat32 = time_heuristics(&start,&stop);

 FILE * outfile = fopen("c.out","w");

 printf("Timings from dot product of a x b\n");
 fprintf(outfile,"Timings from dot product of a x b\n");
 printf("Input time = %.16f for size l = %d\n",deltat32,N);
 fprintf(outfile,"Input time = %.16f for size l = %d\n",deltat32,N);

//defining strides
 int strides[STRIDES] = {1,2,3,5,10,100,1000};
 int i;
 for(i =0;i<STRIDES;++i){
   printf("Performing dot product for %d, stride = %d\n",i+1,strides[i]);
   fprintf(outfile,"Performing dot product for %d, stride = %d\n",i+1,strides[i]);
   clock_gettime(CLOCK_MONOTONIC,&start);//put measuring code after io.
   double dot_prod = dot_product_stride(N,a_in,b_in,strides[i]);
   clock_gettime(CLOCK_MONOTONIC,&stop);
   deltat54 = time_heuristics(&start,&stop);

   printf("Results from the dot product of a x b = %.16le\n",dot_prod);
   fprintf(outfile,"Results from the dot product of a x b = %.16le\n",dot_prod);
   printf("Dot product time %.16le\n",deltat54);
   fprintf(outfile,"Dot product time %.16le\n",deltat54);
 }

 //print out the results
 //cleanup
 free(a_in->data);
 free(b_in->data);
 free(a_in);
 free(b_in);

 fclose(outfile);
}
