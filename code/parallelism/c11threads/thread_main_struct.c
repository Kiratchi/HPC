#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

// To gain flexibility one can define a structure that groups further variables
// and then submit to the created thread a pointer to one or several of these
// structures. This is as powerful as the usual passing of arguments to
// functions.
typedef struct {
  const double *v;
  int ib;
  int ie;
  double *sum;
  mtx_t *mtx;
} thrd_info_t;
// The name thrd_info_t of any standard, so any other name can be used here.

int
main_thrd(
    void *args
    )
{
  const thrd_info_t *thrd_info = (thrd_info_t*) args;
  const double* v = thrd_info->v;
  const int ib = thrd_info->ib;
  const int ie = thrd_info->ie;

  double sum = 0.;
  for ( int ix = ib; ix < ie; ++ix )
    sum += v[ix];

  // When writing to memory that other threads might be access as well, it has
  // to be "protected". A typical technique is to lock a mutex, which can be
  // locked by at most one thread at a time.
  mtx_lock(thrd_info->mtx);
  *thrd_info->sum += sum;
  mtx_unlock(thrd_info->mtx);

  return 0;
}

int
main()
{
  const int sz = 1783;
  double *v = (double*) malloc(sz*sizeof(double));

  for ( int ix = 0; ix < sz; ++ix )
    v[ix] = ix+1;

  const int nthrds = 8;
  thrd_t thrds[nthrds];
  // Here we array allocate what later will be used as arguments to the thread
  // main functions.
  thrd_info_t thrds_info[nthrds];
  
  // A mutex is a construct to synchronize threads. A mutex can be "locked" by
  // at most one thread at a time. As allocated memory must be freed, mutices
  // must be destroyed after use.
  mtx_t mtx;
  mtx_init(&mtx, mtx_plain);

  double sum = 0.;
  const int szloc = sz / nthrds;
  for ( int tx = 0, ib = 0; tx < nthrds; ++tx, ib += szloc ) {
    thrds_info[tx].v = v;
    thrds_info[tx].ib = ib;
    thrds_info[tx].ie = tx != nthrds - 1 ? ib + szloc : sz;
    // Both the sum and the mutex variable point to the same memory in all
    // threads, and are therefore shared memory.
    thrds_info[tx].sum = &sum;
    thrds_info[tx].mtx = &mtx;

    int r = thrd_create(thrds+tx, main_thrd, (void*) (thrds_info+tx));
    if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
  }

  for ( int tx = 0; tx < nthrds; ++tx ) {
    int r;
    thrd_join(thrds[tx], &r);
  }

  mtx_destroy(&mtx);

  printf("sum diff: %f\n", sum - sz * (double)(sz+1) / 2);

  return 0;
}
