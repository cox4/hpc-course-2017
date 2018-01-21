#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2
const int M = 10;
const int N = 2;

int A [M][N] = { {1,4},
		 {2,5},
		 {3,6},
		 {1,2},
		 {1,2},
		 {1,2},
		 {3,4},
		 {9,5},
		 {9,3},
		 {1,2}};
int X [N] = { 3, 2 };
int Y [M];

// HW3: Implement this function.
void* nth_mat_vect(void *rank) {
  // Hint: Look in the slides and/or book.
  return NULL;
}

// Helper method to print vector to standard out.
void printVector(int vector[], int n){
  for(int i=0; i<n; i++){
    printf("%d \n", vector[i]);
  }
}

// Helper method to prints matrix to standard out.
void printMatrix(int matrix[M][N], int m, int n){
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

int main() {
  pthread_t ids[NUM_THREADS];
  int ranks[NUM_THREADS];
  for (int i=0; i < NUM_THREADS; i++) {
    ranks[i] = i;
    // HW3: Create the thread(s) here, calling nth_mat_vec.
  }
  for (int i=0; i < NUM_THREADS; i++) {
    // HW3: Join all the threads here.
  }
  printf("MATRIX A=\n");
  printMatrix(A, N, M);
  printf("VECTOR X=\n");
  printVector(X, N);
  printf("VECTOR Y=\n");
  printVector(Y, M);
  return 0;
}
