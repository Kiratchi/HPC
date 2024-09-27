#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


void
allocate_init_matrix(
    double **m,
    const int szr,
    const int szc
    )
{
  for ( int ix = 0; ix < szr; ++ix ) {
    #pragma omp task shared(m)
    {
      m[ix] = (double*) malloc(szc*sizeof(double));
      for ( int jx = 0; jx < szc; ++jx )
        m[ix][jx] = ix * jx;
    }
  }
}

void
memzero_binary(
    double *v,
    const int sz
    )
{
  if ( sz < 1 )
    return;
  else if (sz == 1 )
    *v = 0.;
  else {
    #pragma omp task shared(v) final(sz < 10)
    memzero_binary(v, sz/2);
    #pragma omp task shared(v) final(sz < 10)
    memzero_binary(v + sz/2, sz - sz/2);
  }
}

int
main()
{
  const int sz = 1000;
  double **m = (double**) malloc(sz*sizeof(double*));
  double *w = (double*) malloc(sz*sizeof(double));


  // The number of threads used by default heavily depends on the
  // circumstances.
  omp_set_num_threads(4);


  // task can (but are not guaranteeed to) be executed in a different thread.
  // Synchronizaton can be achieve via taskwait or via a taskgroup construct,
  // similar to the sections construct.

  #pragma omp parallel
  {
    #pragma omp single
    {
      #pragma omp task shared(m)
      allocate_init_matrix(m, sz/2, sz);

      #pragma omp task shared(m)
      allocate_init_matrix(m + sz/2, sz - sz/2, sz);

      // taskwait only waits for child tasks for the currenct task scope (the
      // single construct in this case). It is implicit at the end of each
      // worksharing (non-task) construct.
      #pragma omp taskwait

      // taskgroup can be useful to conclude substeps
      // #pragma omp taskgroup
    }


    #pragma omp master taskloop
    for ( int ix = 0; ix < sz; ++ix ) {
      double sum = 0.;
      #pragma omp taskloop reduction(+:sum)
      for ( int jx = -1; jx < sz; ++jx )
        sum += m[ix][jx];
      w[ix] = sum;
    }
  }

  printf("w[1] = %f\n", w[1]);


  #pragma omp parallel master
  {
    #pragma omp task shared(w) depend(out: w)
    memzero_binary(w, sz);
    #pragma omp task shared(w) depend(in: w)
    memzero_binary(m[0], sz);
  }

  printf("w[1] = %f\n", w[1]);


  for ( int ix = 0; ix < sz; ++ix )
    free(m[ix]);
  free(m);
  free(w);

  return 0;
}
