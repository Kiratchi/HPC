#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// try float double int long
#define TYPE long
#define RANDTYPE rand_long

// try + * /
#define OP +

#define SZ 10000

float
rand_float()
{
    return (float)rand()/RAND_MAX * 2. - 1.;
}

double
rand_double()
{
    return (double)rand()/RAND_MAX * 2. - 1.;
}

int
rand_int()
{
    return rand();
}

long int
rand_long()
{
    return (long int)rand();
}


int
main(
    int argc,
    char *argv[]
    )
{
  const long int bench_iter = 20000;
  struct timespec bench_start_time;
  struct timespec bench_stop_time;
  double bench_diff_time;

  TYPE *a = (TYPE*) malloc(SZ*sizeof(TYPE));
  TYPE *b = (TYPE*) malloc(SZ*sizeof(TYPE));
  TYPE *c = (TYPE*) malloc(SZ*sizeof(TYPE));
  TYPE *r = (TYPE*) malloc(SZ*sizeof(TYPE));

  for ( int ix = 0; ix < SZ; ++ix ) {
    a[ix] = RANDTYPE();
    b[ix] = RANDTYPE();
    c[ix] = RANDTYPE();
  }

  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < SZ; ++ix )
      r[ix] = a[ix] OP b[ix] + c[ix];

  timespec_get(&bench_start_time, TIME_UTC);

  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < SZ; ++ix )
      r[ix] = a[ix] OP b[ix] + c[ix];

  timespec_get(&bench_stop_time, TIME_UTC);
  { volatile TYPE tmp;
    for ( int ix = 0; ix < SZ; ++ix )
      tmp = r[ix];
  }

  free(a);
  free(b);
  free(c);
  free(r);

  bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("benchmark time for one iteration: %fmus\n",
      bench_diff_time / bench_iter);

  return 0;
}

/* Example runs yield

long int +

O0: benchmark time for one iteration: 42.324600mus
O1: benchmark time for one iteration: 11.881500mus
O2: benchmark time for one iteration: 11.296300mus
O3: benchmark time for one iteration: 4.665300mus
O2nat: benchmark time for one iteration: 12.160050mus
O3nat: benchmark time for one iteration: 4.446950mus

double +

O0: benchmark time for one iteration: 60.224200mus
O1: benchmark time for one iteration: 20.557150mus
O2: benchmark time for one iteration: 24.847000mus
O3: benchmark time for one iteration: 4.980250mus
O2nat: benchmark time for one iteration: 24.189800mus
O3nat: benchmark time for one iteration: 5.224200mus

long int *

O0: benchmark time for one iteration: 62.171300mus
O1: benchmark time for one iteration: 23.366400mus
O2: benchmark time for one iteration: 23.635200mus
O3: benchmark time for one iteration: 0.000600mus
O2nat: benchmark time for one iteration: 23.100800mus
O3nat: benchmark time for one iteration: 0.002250mus

double *

O0: benchmark time for one iteration: 62.006650mus
O1: benchmark time for one iteration: 19.640100mus
O2: benchmark time for one iteration: 20.134000mus
O3: benchmark time for one iteration: 0.001350mus
O2nat: benchmark time for one iteration: 17.336650mus
O3nat: benchmark time for one iteration: 0.000650mus

long int /

O0: benchmark time for one iteration: 220.375100mus
O1: benchmark time for one iteration: 169.285950mus
O2: benchmark time for one iteration: 165.466750mus
O3: benchmark time for one iteration: 0.007450mus
O2nat: benchmark time for one iteration: 165.752650mus
O3nat: benchmark time for one iteration: 0.007400mus

double /

O0: benchmark time for one iteration: 40.373900mus
O1: benchmark time for one iteration: 16.752050mus
O2: benchmark time for one iteration: 17.629900mus
O3: benchmark time for one iteration: 0.000650mus
O2nat: benchmark time for one iteration: 18.546100mus
O3nat: benchmark time for one iteration: 0.000650mus

*/
