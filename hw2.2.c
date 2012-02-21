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

double matrix_ref(int N, int i, int j, vector * m_in){
  int position = N*i + j;
  double val_to_return;
  val_to_return = m_in->data[position];
  return val_to_return;
};

void transpose(int N, vector * a_in, vector * at_in){
  int i,j;
for(j=0;j<N;++j){
	for(i=0;i<N;++i){
	   at_in->data[j*N+i] = a_in->data[i*N+j];
	}
}
}

double matrix_mult(int N,vector * a_in, vector * b_in,vector * c_out){
  int i,j,k;
  for(i=0;i<N;++i){
   for(j =0;j < N;++j){
     double sum=0;	
    	for(k=0;k<N;++k){
	  sum = sum + a_in->data[i*N + k]*b_in->data[j*N + k];
	  }
        c_out->data[i*N + j] = sum;
    }
  }
}


int main(void){
  int l = 1000;

  struct timespec start,stop;
  double deltat1,deltat2,deltat3;
  
  //Reading data file "a.in"
  printf(" Reading a\n");
  clock_gettime(CLOCK_MONOTONIC,&start);
  vector *a_in = readfile("a.in");
  vector *at_in;
  at_in = (vector *) malloc(sizeof(vector));
  int at_size = a_in->size;
  at_in->size = at_size;
  at_in->allocSize = a_in->allocSize;
  double * at_data;
  at_data = (double *) calloc(at_size,sizeof(double));
  at_in->data = at_data;
  transpose(l,a_in,at_in);
  //Reading data file "b.in"
  printf(" Reading b\n");
  vector *b_in = readfile("b.in");
  vector * c_out;
  c_out = (vector *) malloc(sizeof(vector));
  int c_size = b_in->size;
  c_out->size = c_size;
  c_out->allocSize = b_in->allocSize;
  double * c_data;
  c_data = (double *) calloc(c_size,sizeof(double));
  c_out->data = c_data;
  clock_gettime(CLOCK_MONOTONIC,&stop);
  deltat1 = time_heuristics(&start,&stop);

  FILE * outfile = fopen("c.out","w");

  printf("Performing matrix multiply\n");

 clock_gettime(CLOCK_MONOTONIC,&start);
 matrix_mult(l,at_in,b_in,c_out);
 clock_gettime(CLOCK_MONOTONIC,&stop);
 deltat2 = time_heuristics(&start,&stop);
 double val_out = matrix_ref(l,0,0,c_out);
 int middle = l / 2;
 double val_out2 = matrix_ref(l,middle-1,middle-1,c_out);
 double val_out3 = matrix_ref(l,l-1,l-1,c_out);
 printf("Results from matrix multiply of a x b of size l= %d\n",l);
 printf("c(1,1) = %.16f\n",val_out);
 printf("c(%d,%d) = %.16f\n",middle,middle,val_out2);
 printf("c(%d,%d) = %.16f\n",l,l,val_out3);

 clock_gettime(CLOCK_MONOTONIC,&start);
 int i,j;
 for(i=0;i<l;++i){
   for(j=0;j<l;++j){
     double temp_val = matrix_ref(l,i,j,c_out);
     fprintf(outfile,"%.16f,",temp_val);
   }
   fprintf(outfile,"\n");
 }

 fprintf(outfile,"Results from matrix multiply of a x b of size l= %d\n",l);
 fprintf(outfile,"c(1,1) = %.16f\n",val_out);
 fprintf(outfile,"c(%d,%d) = %.16f\n",middle,middle,val_out2);
 fprintf(outfile,"c(%d,%d) = %.16f\n",l,l,val_out3);
 clock_gettime(CLOCK_MONOTONIC,&stop);
 deltat3 = time_heuristics(&start,&stop); 

 fprintf(outfile,"Timings from matrix multipy of a x b of size l = %d\n",l);
 fprintf(outfile,"Input time = %.16f\n",deltat1);
 fprintf(outfile,"Matrix multiply time %.16f\n",deltat2);
 fprintf(outfile,"Output time for writing matrix c %.16f\n",deltat3);

 printf("Timings from matrix multipy of a x b of size l = %d\n",l);
 printf("Input time = %.16f\n",deltat1);
 printf("Matrix multiply time %.16f\n",deltat2);
 printf("Output time for writing matrix c %.16f\n",deltat3);
 

 free(a_in->data);
 free(b_in->data);
 free(c_out->data);
 free(at_in->data);
 free(a_in);
 free(at_in);
 free(b_in);
 free(c_out);

 fclose(outfile);
}
