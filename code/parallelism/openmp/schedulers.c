#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
  const int sz = 1000;
  double *v = (double*) malloc(sz*sizeof(double));
  double *w = (double*) malloc(sz*sizeof(double));

  for ( int ix = 0; ix < sz; ++ix )
    v[ix] = ix+1;

  // The number of threads used by default heavily depends on the
  // circumstances.
  omp_set_num_threads(4);


  // The distribution of for the elements in a parrallel for construct is
  // handled by a scheduler.

  // static divides the loop into chunks of constant size (expect for possibly
  // the last one), and associated to threads in fixed order.
  // Good choice for problems that are divided up into pieces of about equal
  // runtime.
  #pragma omp parallel for schedule(static, 10)
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  // dynamic also divides the loop into chunks of constant size (expect for possibly
  // the last one), but they are associated to threads whenever a thread is idle.
  // Good if execution time for iterations is hard to estimate or not about equal.
  #pragma omp parallel for schedule(dynamic, 10)
  for ( int ix = 0; ix < sz - 4; ++ix ) {
    int isz = 1 + ix % 4;
    w[ix] = 0.;
    for ( int jx = ix; jx < ix+isz; ++jx )
      w[ix] = 2*v[jx];
  }

  printf("w[0] = %f\n", w[0]);


  // guided divides the loop into chunks of adaptable (decreasing) size, and
  // they are associated to threads whenever a thread is idle.
  // Good choice if variation of execution time is significant or if individual
  // interations take a long time.
  #pragma omp parallel for schedule(guided)
  for ( int ix = 0; ix < sz; ++ix ) {
    w[ix] = 0.;
    for ( int jx = ix; jx < sz; ++jx )
      w[ix] = 2*v[jx];
  }

  printf("w[0] = %f\n", w[0]);


  // runtime decides the scheduler based on the environment variable
  // OMP_SCHEDULE.
  // For example,  OMP_SCHEDULE=dynamic ./schedulers
  #pragma omp parallel for schedule(runtime)
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  free(v);
  free(w);

  return 0;
}
