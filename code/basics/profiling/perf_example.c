#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// try float double int long
#define TYPE long int
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
