/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Instructor Bryan Mills, PhD (bmills@cs.pitt.edu)
 * Students:
 * Implement openmp verions of conway's game of life.
 */

#include "timer.h"
#include "io.h"

// Gets the value of a particular position in the matrix, considering boundary conditions
int value(int **matrix, int i, int j, int N){
  if(i<0 || i>=N || j<0 || j>=N)
    return 0;
  else
    return matrix[i][j];
}

// Gets the number of live neighbors
inline int neighbors(int **matrix, int i, int j, int N){
  return value(matrix,i-1,j-1,N) + value(matrix,i-1,j,N) + value(matrix,i-1,j+1,N) +
    value(matrix,i,j-1,N) + value(matrix,i,j+1,N) +
    value(matrix,i+1,j-1,N) + value(matrix,i+1,j,N) + value(matrix,i+1,j+1,N); 
}

// Function implementing Conway's Game of Life
void conway(int **World, int N, int M){
  int **next, **matrix, **tmp;

  // allocating temporary matrix
  next = new int*[N];
  matrix = new int*[N];
  for (int i=0; i<N; i++){
    next[i] = new int[N];
    matrix[i] = new int[N];
  }

  // copying data into matrix
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      matrix[i][j] = World[i][j];
    }
  }

  // iterating over all generations
  for(int gen=0; gen<M; gen++){
    #pragma omp parallel for
    for(int i=0; i<N; i++){
      #pragma omp parallel for
      for(int j=0; j<N; j++){
	int total = neighbors(matrix,i,j,N);
	int val = matrix[i][j];
	switch(total){
	case 0:
	case 1:
	  val = 0;
	  break;
	case 2:
	  break;
	case 3:
	  if(!val) val = 1;
	  break;
	default:
	  val = 0;
	}
	next[i][j] = val;
      }
    }

    // switching matrices
    tmp = next;
    next = matrix;
    matrix = tmp;
  }

  // copying data into matrix
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      World[i][j] = matrix[i][j];
    }
  }

  // releasing memory
  for (int i=0; i<N; i++) {
    delete [] next[i];
    delete [] matrix[i];
  }
  delete [] next;
  delete [] matrix;
}

// Allocate square matrix.
int **allocMatrix(int size) {
  int **matrix;
  matrix = (int **)malloc(size * sizeof(int *));
  for (int row = 0; row < size; row++) {
    matrix[row] = (int *)malloc(size * sizeof(int));
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

// Main method      
int main(int argc, char* argv[]) {
  int N,M;
  int **World;
  double elapsedTime;

  // checking parameters
  if (argc != 3 && argc != 4) {
    printf("Parameters: <N> <M> [<file>]\n");
    return 1;
  }
  N = atoi(argv[1]);
  M = atoi(argv[2]);

  // allocating matrices
  World = allocMatrix(N);

  // reading files (optional)
  if(argc == 4){
    readMatrixFile(World,N,argv[3]);
  } else {
    // Otherwise, generate two random matrix.
    srand (time(NULL));
    for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
	World[i][j] = rand() % 2;
      }
    }
  }

  // starting timer
  timerStart();

  // calling conway's game of life 
  conway(World,N,M);

  // stopping timer
  elapsedTime = timerStop();

  printMatrix(World,N);

  printf("Took %ld ms\n", timerStop());

  // releasing memory
  for (int i=0; i<N; i++) {
    delete [] World[i];
  }
  delete [] World;

  return 0;
}
