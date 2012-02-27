/***************************
  Conway Game of Life

  iGhostOnetialize and finalize MPI;
  run duplicate calculation
  on each processor

****************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ArrayOne 200  /* array sizes */
#define ArrayTwo 200
#define NSTEPS 500    /* number of time steps */

void main(int argc, char *argv[]){

  int i, j, n, im, ip, jm, jp, GhostOne, GhostTwo, nsum, isum, nProcs, procID;
  int **oldState, **newState;  
  float x;

  /* initialize MPI */

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&nProcs);
  MPI_Comm_rank(MPI_COMM_WORLD,&procID);

  /* allocate arrays */

  GhostOne = ArrayOne + 2;  /* add 2 for left and right ghost cells */
  GhostTwo = ArrayTwo + 2;

  oldState = malloc(GhostOne*sizeof(int*));
  newState = malloc(GhostOne*sizeof(int*));

  for(i=0; i<GhostOne; i++) {
    oldState[i] = malloc(GhostTwo*sizeof(int));
    newState[i] = malloc(GhostTwo*sizeof(int));
  }

  /*  iGhostOnetialize elements of oldState to 0 or 1 */

  for(i=1; i<=ArrayOne; i++) {
    for(j=1; j<=ArrayTwo; j++) {
      x = rand()/((float)RAND_MAX + 1);
      if(x<0.5) {
        oldState[i][j] = 0;
      }
      else {
        oldState[i][j] = 1;
      }
    }
  }

  /*  time steps */

  for(n=0; n<NSTEPS; n++) {

    /* corner boundary conditions */
    
    oldState[0][0] = oldState[ArrayOne][ArrayTwo];
    oldState[0][ArrayTwo+1] = oldState[ArrayOne][1];
    oldState[ArrayOne+1][ArrayTwo+1] = oldState[1][1];
    oldState[ArrayOne+1][0] = oldState[1][ArrayTwo];
    
    /* left-right boundary conditions */
    
    for(i=1; i<=ArrayOne; i++) {
      oldState[i][0] = oldState[i][ArrayTwo];
      oldState[i][ArrayTwo+1] = oldState[i][1];
    }

    /* top-bottom boundary conditions */

    for(j=1; j<=ArrayTwo; j++) {
      oldState[0][j] = oldState[ArrayOne][j];
      oldState[ArrayOne+1][j] = oldState[1][j];
    }

    for(i=1; i<=ArrayOne; i++) {
      for(j=1; j<=ArrayTwo; j++) {

        im = i-1;
        ip = i+1;
        jm = j-1;
        jp = j+1;
        nsum =  oldState[im][jp] + oldState[i][jp] + oldState[ip][jp]
          + oldState[im][j ]              + oldState[ip][j ] 
          + oldState[im][jm] + oldState[i][jm] + oldState[ip][jm];

        switch(nsum) {
        case 3:
          newState[i][j] = 1;
          break;
        case 2:
          newState[i][j] = oldState[i][j];
          break;
        default:
          newState[i][j] = 0;
        }
      }
    }

    /* copy newState state into oldState state */

    for(i=1; i<=ArrayOne; i++) {
      for(j=1; j<=ArrayTwo; j++) {
        oldState[i][j] = newState[i][j];
      }
    }
  }

  /*  Iterations are done; sum the number of live cells */

  isum = 0;
  for(i=1; i<=ArrayOne; i++) {
    for(j=1; j<=ArrayTwo; j++) {
      isum = isum + newState[i][j];
    }
  }

 
  /* print process identification and nunmber of live cells*/

  printf("Process %d of %d:  Number of live cells = %dnn", procID, nProcs, isum);

  MPI_Finalize();

}
