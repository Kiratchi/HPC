#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
  const int sz = 1000;
  double *v = (double*) aligned_alloc(8*sizeof(double), sz*sizeof(double));
  double *w = (double*) aligned_alloc(8*sizeof(double), sz*sizeof(double));
  double sum;

  for ( int ix = 0; ix < sz; ++ix )
    v[ix] = ix+1;

  // The number of threads used by default heavily depends on the
  // circumstances.
  omp_set_num_threads(4);


  #pragma omp simd simdlen(8) aligned(v,w:8)
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  w[0] = 0.;
  // Parallel for simd combines parallel for and simd constructs.
  #pragma omp parallel for simd simdlen(8) aligned(v,w:8)
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  sum = 0.;
  #pragma omp simd simdlen(8) aligned(v:8) reduction(+:sum)
  for ( int ix = 0; ix < sz; ++ix )
    sum += v[ix];

  printf("sum = %f\n", sum);


  sum = 0.;
  #pragma omp parallel for simd simdlen(8) aligned(v:8) reduction(+:sum)
  for ( int ix = 0; ix < sz; ++ix )
    sum += v[ix];

  printf("sum = %f\n", sum);


  free(v);
  free(w);

  return 0;
}
