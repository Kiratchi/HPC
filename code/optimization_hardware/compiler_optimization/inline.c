/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
static inline
double
rand_double()
{
    return(double)rand()/RAND_MAX * 2. - 1.;
}
*/

/*
inline
double
rand_double()
{
    return(double)rand()/RAND_MAX * 2. - 1.;
}
*/

/*
// It might seem advised to provide this defintion, when the linker fails to
// find the reference for rand_double, but this is yields a compiler error.
double
rand_double()
{
    return(double)rand()/RAND_MAX * 2. - 1.;
}
*/

int
main(
    int argc,
    char *argv[]
    )
{
  const int sz = 10000;
  const long int bench_iter = 20000;
  struct timespec bench_start_time;
  struct timespec bench_stop_time;
  double bench_diff_time;

  double *a = (double*) malloc(sz*sizeof(double));
  double *b = (double*) malloc(sz*sizeof(double));

  for ( int ix = 0; ix < sz; ++ix )
    a[ix] = rand_double();

  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < sz; ++ix )
      b[ix] = a[ix] * a[ix];


  timespec_get(&bench_start_time, TIME_UTC);

  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < sz; ++ix )
      b[ix] = a[ix] * a[ix];

  timespec_get(&bench_stop_time, TIME_UTC);

  { volatile double tmp;
    for ( int ix = 0; ix < sz; ++ix )
      tmp = b[ix];
  }

  free(a);
  free(b);

  bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("benchmark time for one iteration: %fmus\n",
      bench_diff_time / bench_iter);

  return 0;
}
