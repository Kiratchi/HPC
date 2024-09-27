#define _XOPEN_SOURCE 700

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

int
main(
    int argc,
    char * argv[]
    )
{
  MPI_Init(&argc, &argv);

  int nmb_mpi_proc, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &nmb_mpi_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  if (mpi_rank==0) {
    printf( "Number of processes: %d\n", nmb_mpi_proc );

    int msg_send = 1; int len_send = 1; int dest_rank = 1; int tag_send = 1;
    int msg_recv;     int len_recv = 1; int src_rank  = 1; int tag_recv = 1;
    MPI_Status status;

    /* data types:
     * MPI_INT, MPI_UNSIGNED, MPI_LONG, MPI_UNSIGNED_LONG,
     * MPI_FLOAT, MPI_DOUBLE, MPI_LONG_DOUBLE, or
     * a custom MPI_Datatype constructed using MPI_Type_create_struct.
     * tags: use MPI_ANY_TAG To receive messages with arbitrary tags.
     */
    MPI_Sendrecv(&msg_send, len_send, MPI_INT, dest_rank, tag_send,
                 &msg_recv, len_recv, MPI_INT, src_rank,  tag_recv,
                 MPI_COMM_WORLD, &status);

    // status: a structure with at least the following fields
    // MPI_SOURCE, MPI_TAG, MPI_ERROR.
    int sz_recv;
    MPI_Get_count(&status, MPI_INT, &sz_recv);
    if ( sz_recv != len_recv ) {
      fprintf(stderr, "did not receive 1 integer\n");
      exit(1);
    }

    printf( "MPI message sent from %d: %d\n", mpi_rank, msg_send );
    printf( "MPI message received at %d: %d\n", mpi_rank, msg_recv );
  }
  
  else if (mpi_rank==1) {
    int msg_send = 2; int len_send = 1; int dest_rank = 0; int tag_send = 1;
    int msg_recv;     int len_recv = 1; int src_rank  = 0; int tag_recv = 1;
    MPI_Status status;
    
    MPI_Sendrecv(&msg_send, len_send, MPI_INT, dest_rank, tag_send,
                 &msg_recv, len_recv, MPI_INT, src_rank,  tag_recv,
                 MPI_COMM_WORLD, &status);

    int sz_recv;
    MPI_Get_count(&status, MPI_INT, &sz_recv);
    if ( sz_recv != len_recv ) {
      fprintf(stderr, "did not receive 1 integer\n");
      exit(1);
    }

    printf( "MPI message sent from %d: %d\n", mpi_rank, msg_send );
    printf( "MPI message received at %d: %d\n", mpi_rank, msg_recv );
  }

  MPI_Finalize();


  return 0;
}
