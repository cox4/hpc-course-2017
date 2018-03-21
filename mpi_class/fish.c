#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */

const int MAX_STRING = 100;

int main(void) {
  char       greeting[MAX_STRING];  /* String storing message */
  int        comm_sz;               /* Number of processes    */
  int        my_rank;               /* My process rank        */

  /* Start up MPI */
  MPI_Init(NULL, NULL);

  /* Get the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  /* Get my rank among all the processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank != 0) {
    /* Create message */
    /* Send message to process 0 */
    printf("Printing from %d\n", my_rank);
    sprintf(greeting, "Greetings from process %d of %d!",
            my_rank, comm_sz);
    MPI_Send(&greeting, 32, MPI_CHAR, 0, 0,
	     MPI_COMM_WORLD);
  } else {
    /* Print my message */
    int q = 1;
    int other_rank;
    int my_c;
    printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
    for (q = 1; q < comm_sz; q++) {
      /* Receive message from process q */
      MPI_Status  status;
      MPI_Recv(greeting, 100, MPI_CHAR, q,
	       0, MPI_COMM_WORLD, &status);
      /* Print message from process q */
      MPI_Get_count(&status, MPI_CHAR, &my_c);
      printf("I got something %d from of length %d and value:\n%s\n", status.MPI_SOURCE, my_c,
	     greeting);
    }
  }

  /* Shut down MPI */
  MPI_Finalize();

  return 0;
}  /* main */
