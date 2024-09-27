#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <threads.h>


typedef struct {
  atomic_int *nthrds_completed;
  atomic_char *thrds_block_even;
  atomic_char *thrds_block_odd;
  int nthrds;
  int tx;
  int niter;
  int sz;
  int *v;
  int *w;
} thrd_info_t;

int
main_thrd(
    void *args
    )
{
  const thrd_info_t *thrd_info = (thrd_info_t*) args;
  atomic_int *nthrds_completed = thrd_info->nthrds_completed;
  atomic_char *thrds_block_even = thrd_info->thrds_block_even;
  atomic_char *thrds_block_odd = thrd_info->thrds_block_odd;
  const int nthrds = thrd_info->nthrds;
  const int tx = thrd_info->tx;
  const int niter = thrd_info->niter;
  int *v = thrd_info->v;
  int *w = thrd_info->w;

  for ( int lx = 0; lx < niter; ++lx ) {
    for ( int ix = 0; ix < nthrds; ++ix )
      if ( v[ix] != lx ) {
        fprintf(stderr, "failed v[%i] = %i, %i\n", ix, v[ix], lx);
        thrd_exit(1);
      }

    w[tx] = v[tx] + 1;

    // Swap pointers v and w.
    { int *tmp = v;
      v = w;
      w = tmp;
    }

    // Synchronize threads
    if ( nthrds ==
          1 + atomic_fetch_add_explicit(nthrds_completed, 1,
                                        memory_order_relaxed) ) {
      // We can reset the number of completed threads, since we will enforce
      // consistentcy when setting the other flag.
      atomic_store_explicit(nthrds_completed, 0, memory_order_relaxed);

      // We can reset this flag in relaxed order, since we will enforce
      // consistentcy when setting the other flag.
      atomic_store_explicit(thrds_block_odd,  1, memory_order_relaxed);
      atomic_store_explicit(thrds_block_even, 0, memory_order_release);
    } else {
      // We use a spin/busy lock, since we expect that threads take about the
      // same time.
      while ( atomic_load_explicit(thrds_block_even, memory_order_acquire) );
    }

    // Swap pointers to the atomic blockiing variables.
    { atomic_char *tmp = thrds_block_even;
      thrds_block_even = thrds_block_odd;
      thrds_block_odd = tmp;
    }
  }

  return 0;
}


int
main()
{
  const int nthrds = 4;
  thrd_t thrds[nthrds];
  thrd_info_t thrds_info[nthrds];

  const int niter = 1000;

  int *v = (int*) malloc(nthrds*sizeof(int));;
  int *w = (int*) malloc(nthrds*sizeof(int));;

  for ( int ix = 0; ix < nthrds; ++ix )
    v[ix] = 0;

  atomic_int nthrds_completed;
  atomic_char thrds_block_even;
  atomic_char thrds_block_odd;
  atomic_init(&nthrds_completed, 0);
  atomic_init(&thrds_block_even, 1);
  // No need to initialize thrds_block_odd; This only sets the value assuming
  // non-concurrent access.

  for ( int tx = 0; tx < nthrds; ++tx ) {
    thrds_info[tx].nthrds_completed = &nthrds_completed;
    thrds_info[tx].thrds_block_even = &thrds_block_even;
    thrds_info[tx].thrds_block_odd = &thrds_block_odd;
    thrds_info[tx].nthrds = nthrds;
    thrds_info[tx].tx = tx;
    thrds_info[tx].niter = niter;
    thrds_info[tx].v = v;
    thrds_info[tx].w = w;

    int r = thrd_create(thrds+tx, main_thrd, (void*) (thrds_info+tx));
    if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
  }

  for ( int tx = 0; tx < nthrds; ++tx ) {
    int r;
    thrd_join(thrds[tx], &r);
    if ( r != 0 ) {
      fprintf(stderr, "thread %i failed sychnronization test\n", tx);
      exit(1);
    }
  }

  for ( int tx = 0; tx < nthrds; ++tx )
    if ( (niter & 1) ? w[tx] : v[tx] != niter )
      printf("failed final value at thread %i\n", tx);

  free(v);
  free(w);

  return 0;
}

// NOTE: Spin lock can be very fast when not oversubscribing the CPUs. Their
// performance degrades rapidly, if this is not the case. For example on a 4
// core, 8 hyperthread laptop the program with 10 threads runs in 13s in one
// instance.
