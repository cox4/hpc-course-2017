#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3
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

void *nth_mat_vect(void *rank) {
  int *rank_int_ptr = (int*) rank;

  int my_rank = *rank_int_ptr;
  int local_m = M / NUM_THREADS;

  int my_first_row = (my_rank * local_m);
  int my_last_row = (my_rank+1)*local_m - 1;
  int remainder = M % NUM_THREADS;
  // Easiest solution, just give the last thread the extra work.
  if (my_rank == NUM_THREADS -1 
      && my_last_row < M-1 
      && remainder != 0) {
    my_last_row += remainder;
  }

  // Another solution splits extra amongst the first few ranks.
//  if (my_rank < remainder) {
//    my_first_row = (local_m + 1) * my_rank;
//  } else {
//    my_first_row = local_m * my_rank + remainder;
//  }
//  if ((my_rank+1) < remainder) {
//    my_last_row = (local_m + 1) * (my_rank+1);
//  } else {
//    my_last_row = local_m * (my_rank+1) + remainder;
//    // Handle boundary case.
//    if (my_last_row > (M-1)) {
//      my_last_row = (M-1);
//    }
//  }

  printf("%d (%d, %d)\n", my_rank, my_first_row, my_last_row);

  for (int i = my_first_row; i <= my_last_row; i++) {
    Y[i] = 0.0;
    for (int j = 0; j < N; j++) {
      Y[i] += A[i][j] * X[j];
    }
  }
  return NULL;
}

// Prints vector to standard out
void printVector(int vector[], int n){
  for(int i=0; i<n; i++){
    printf("%d \n", vector[i]);
  }
}

// Prints matrix to standard output
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
    pthread_create(&ids[i], NULL, nth_mat_vect, &ranks[i]);
  }
  for (int i=0; i < NUM_THREADS; i++) {
    pthread_join(ids[i], NULL);
  }
  printf("MATRIX A=\n");
  printMatrix(A, N, M);
  printf("VECTOR X=\n");
  printVector(X, N);
  printf("VECTOR Y=\n");
  printVector(Y, M);
  return 0;
}
