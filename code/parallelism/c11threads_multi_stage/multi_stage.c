#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>

typedef struct {
  int val;
  char pad[60]; // cacheline - sizeof(int)
} int_padded;

typedef struct {
  const float **v;
  float **w;
  int ib;
  int istep;
  int sz;
  int tx;
  mtx_t *mtx;
  cnd_t *cnd;
  int_padded *status;
} thrd_info_t;

typedef struct {
  const float **v;
  float **w;
  int sz;
  int nthrds;
  mtx_t *mtx;
  cnd_t *cnd;
  int_padded *status;
} thrd_info_check_t;

int
main_thrd(
    void *args
    )
{
  const thrd_info_t *thrd_info = (thrd_info_t*) args;
  const float **v = thrd_info->v;
  float **w = thrd_info->w;
  const int ib = thrd_info->ib;
  const int istep = thrd_info->istep;
  const int sz = thrd_info->sz;
  const int tx = thrd_info->tx;
  mtx_t *mtx = thrd_info->mtx;
  cnd_t *cnd = thrd_info->cnd;
  int_padded *status = thrd_info->status;

  for ( int ix = ib; ix < sz; ix += istep ) {
    const float *vix = v[ix];
    // We allocate the rows of the result before computing, and free them in another thread.
    float *wix = (float*) malloc(sz*sizeof(float));

    for ( int jx = 0; jx < sz; ++jx )
      wix[jx] = sqrtf(vix[jx]);

    // I DO NOT UNDERSTAND WHAT RÒR WHY THIS WAS IMMPORTANT
    // BUT IT WAS INDEED IMPORTANT!!!
    // Remember what he wrote on the board about threads "wating" on each other?
    mtx_lock(mtx);
    w[ix] = wix;
    status[tx].val = ix + istep;
    mtx_unlock(mtx);
    cnd_signal(cnd);

    // In order to illustrate thrd_sleep and to force more synchronization
    // points, we sleep after each line for one micro seconds.
    thrd_sleep(&(struct timespec){.tv_sec=0, .tv_nsec=1000}, NULL);
  }

  return 0;
}

int
main_thrd_check(
    void *args
    )
{
  const thrd_info_check_t *thrd_info = (thrd_info_check_t*) args;
  const float **v = thrd_info->v;
  float **w = thrd_info->w;
  const int sz = thrd_info->sz;
  const int nthrds = thrd_info->nthrds;
  mtx_t *mtx = thrd_info->mtx;
  cnd_t *cnd = thrd_info->cnd;
  int_padded *status = thrd_info->status;

  const float eps = 1e-1;

  // We do not increment ix in this loop, but in the inner one.
  for ( int ix = 0, ibnd; ix < sz; ) {

    // If no new lines are available, we wait.
    for ( mtx_lock(mtx); ; ) {
      // We extract the minimum of all status variables.
      ibnd = sz;
      for ( int tx = 0; tx < nthrds; ++tx )
        if ( ibnd > status[tx].val )
          ibnd = status[tx].val;

      if ( ibnd <= ix )
        // We rely on spurious wake-ups, which in practice happen, but are not
        // guaranteed.
        cnd_wait(cnd,mtx);
      else {
        mtx_unlock(mtx);
        break;
      }

      // Instead of employing a conditional variable, we could also invoke
      // thrd_yield or thrd_sleep in order to yield to other threads or grant a
      // specified time to the computation threads.
    }

    fprintf(stderr, "checking until %i\n", ibnd);

    // We do not initialize ix in this loop, but in the outer one.
    for ( ; ix < ibnd; ++ix ) {
      // We only check the last element in w, since we want to illustrate the
      // situation where the check thread completes fast than the computaton
      // threads.
      int jx = sz-1;
      float diff = v[ix][jx] - w[ix][jx] * w[ix][jx];
      if ( diff < -eps || diff > eps ) {
        fprintf(stderr, "incorrect compuation at %i %i: %f %f %f\n",
            ix, jx, diff, v[ix][jx], w[ix][jx]);
        // This exists the whole program, including all other threads.
        exit(1);
      }

      // We free the component of w, since it will never be used again.
      free(w[ix]);
    }
  }

  return 0;
}

int
main()
{
  const int sz = 500;

  float **v = (float**) malloc(sz*sizeof(float*));
  float **w = (float**) malloc(sz*sizeof(float*));
  float *ventries = (float*) malloc(sz*sz*sizeof(float));
  // The entries of w will be allocated in the computation threads are freed in
  // the check thread.

  for ( int ix = 0, jx = 0; ix < sz; ++ix, jx += sz )
    v[ix] = ventries + jx;

  for ( int ix = 0; ix < sz*sz; ++ix )
    ventries[ix] = ix;

  const int nthrds = 8;
  thrd_t thrds[nthrds];
  thrd_info_t thrds_info[nthrds];

  thrd_t thrd_check;
  thrd_info_check_t thrd_info_check;
  
  mtx_t mtx;
  mtx_init(&mtx, mtx_plain);

  cnd_t cnd;
  cnd_init(&cnd);

  int_padded status[nthrds];

  for ( int tx = 0; tx < nthrds; ++tx ) {
    thrds_info[tx].v = (const float**) v;
    thrds_info[tx].w = w;
    thrds_info[tx].ib = tx;
    thrds_info[tx].istep = nthrds;
    thrds_info[tx].sz = sz;
    thrds_info[tx].tx = tx;
    thrds_info[tx].mtx = &mtx;
    thrds_info[tx].cnd = &cnd;
    thrds_info[tx].status = status;
    status[tx].val = -1;

    int r = thrd_create(thrds+tx, main_thrd, (void*) (thrds_info+tx));
    if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
    thrd_detach(thrds[tx]);
  }

  {
    thrd_info_check.v = (const float**) v;
    thrd_info_check.w = w;
    thrd_info_check.sz = sz;
    thrd_info_check.nthrds = nthrds;
    thrd_info_check.mtx = &mtx;
    thrd_info_check.cnd = &cnd;
    // It is important that we have initialize status in the previous for-loop,
    // since it will be consumed by the check threads.
    thrd_info_check.status = status;

    int r = thrd_create(&thrd_check, main_thrd_check, (void*) (&thrd_info_check));
    if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
  }

  {
    int r;
    thrd_join(thrd_check, &r);
  }

  free(ventries);
  free(v);
  free(w);

  mtx_destroy(&mtx);
  cnd_destroy(&cnd);

  return 0;
}
