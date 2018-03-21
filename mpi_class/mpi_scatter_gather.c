#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */
#include <time.h>

const int MAX_STRING = 10000;
const int RAND_MAX = 10;

void Generate_vector(
      double x[] /* out */, 
      int    n   /* in  */) {
   int i;

   for (i = 0; i < n; i++)
     x[i] = ((double) rand())/((double) RAND_MAX);
}

int main(void) {
  char       greeting[MAX_STRING];  /* String storing message */
  int        comm_sz;               /* Number of processes    */
  int        my_rank;               /* My process rank        */

  /* Start up MPI */
  MPI_Init(NULL, NULL);


  srand(time(NULL));

  /* Get the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* Get my rank among all the processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int n = 12;
  double* x = malloc(n*sizeof(double));

  int local_n = n / comm_sz;
  double* local_x = malloc(local_n*sizeof(double));

  if (my_rank == 0) {
    Generate_vector(x, n);

  int i = 0;
  for(i = 0; i < n; i++) {
    printf("pid: %d x[ %d ] = %f\n", my_rank, i, x[i]);
  }
  
  MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else{
    MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  int i = 0;
  for(i = 0; i < local_n; i++) {
    printf("pid: %d local_x[ %d ] = %f\n", my_rank, i, local_x[i]);
    local_x[i] = local_x[i] + 1;
    printf("**new pid: %d local_x[ %d ] = %f\n", my_rank, i, local_x[i]);
  }

  double* rec_buf;
  rec_buf = malloc(n*sizeof(double));

  MPI_Allgather(local_x, local_n, MPI_DOUBLE, rec_buf, local_n, MPI_DOUBLE, MPI_COMM_WORLD);

  int j = 0;
  for(j = 0; j < n; j++) {
    printf("pid: %d rec_buf[ %d ] = %f\n", my_rank, j, rec_buf[j]);
  }


  if (my_rank != 0) {
    /* Create message */
    sprintf(greeting, "Greetings from process %d of %d!",
            my_rank, comm_sz);
    /* Send message to process 0 */
    MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0,
	     MPI_COMM_WORLD);
  } else {
    /* Print my message */
    int q = 1;
    printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
    for (q = 1; q < comm_sz; q++) {
      MPI_Status status;
      /* Receive message from process q */
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q,
	       0, MPI_COMM_WORLD, &status);
      /* Print message from process q */
      printf("FROM rank: %d msg: %s\n", status.MPI_SOURCE, greeting);
    }
  }

  /* Shut down MPI */
  MPI_Finalize();

  return 0;
}  /* main */
