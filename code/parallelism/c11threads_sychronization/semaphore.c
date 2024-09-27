#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>


typedef struct {
  mtx_t mtx;
  cnd_t cnd;
  size_t cnt;
} smphr_t;


int
smphr_init(
    smphr_t* smphr,
    size_t count
    )
{
  smphr->cnt = count;

  int r;
  r = mtx_init(&smphr->mtx, mtx_plain);
  if ( r == thrd_success )
    r = cnd_init(&smphr->cnd);

  return r;
}

void
smphr_destroy(
    smphr_t* smphr
    )
{
  mtx_destroy(&smphr->mtx);
  cnd_destroy(&smphr->cnd);
}

void
smphr_release(
    smphr_t* smphr
    )
{
  mtx_lock(&smphr->mtx);

  ++smphr->cnt;
  cnd_signal(&smphr->cnd);

  mtx_unlock(&smphr->mtx);
}

void
smphr_acquire(
    smphr_t* smphr
    )
{
  mtx_lock(&smphr->mtx);
  while ( smphr->cnt == 0 )
    cnd_wait(&smphr->cnd, &smphr->mtx);

  --smphr->cnt;

  mtx_unlock(&smphr->mtx);
}


typedef struct {
  const long int *v;
  int ib;
  int ie;
  atomic_int_least64_t *sum;
  smphr_t *smphr;
} thrd_info_t;

int
main_thrd(
    void *args
    )
{
  const thrd_info_t *thrd_info = (thrd_info_t*) args;
  const long int* v = thrd_info->v;
  const int ib = thrd_info->ib;
  const int ie = thrd_info->ie;

  smphr_acquire(thrd_info->smphr);
//  mutex_lock(thrd_info->mtx);
  long int sum = 0;
  for ( int ix = ib; ix < ie; ++ix )
    sum += v[ix];
//  mutex_unlock(thrd_info->mtx);
  smphr_release(thrd_info->smphr);

  atomic_fetch_add_explicit(thrd_info->sum, sum, memory_order_relaxed);

  return 0;
}


int
main()
{
  const int sz = 5000000;
  long int *v = (long int*) malloc(sz*sizeof(long int));

  // We do not initialize v, which is actually incorreced, for the sake of
  // making the difference among various possible maximal counts for semaphores
  // clearly visible.

  // for ( int ix = 0; ix < sz; ++ix )
  //   v[ix] = ix+1;


  const int nthrds = 8;
  thrd_t thrds[nthrds];
  thrd_info_t thrds_info[nthrds];
  
  smphr_t smphr;
  smphr_init(&smphr, 1);

  atomic_int_least64_t sum;
  atomic_init(&sum, 0);

  const int szloc = sz / nthrds;
  for ( int tx = 0, ib = 0; tx < nthrds; ++tx, ib += szloc ) {
    thrds_info[tx].v = v;
    thrds_info[tx].ib = ib;
    thrds_info[tx].ie = tx != nthrds - 1 ? ib + szloc : sz;
    thrds_info[tx].sum = &sum;
    thrds_info[tx].smphr = &smphr;

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

  smphr_destroy(&smphr);

  printf("difference: %li\n", atomic_load_explicit(&sum, memory_order_relaxed) - sz * (long int)(sz + 1) / 2);

  return 0;
}
