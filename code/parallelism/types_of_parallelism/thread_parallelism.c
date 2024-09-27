#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

double
rand_double()
{
    return (double)rand()/RAND_MAX * 2. - 1.;
}

int
main()
{
  const int sz = 1000000000;
  double sum;
  double *a, *b;

  #pragma omp parallel num_threads(10)
  {
    #pragma omp single nowait
    a = (double *) malloc(sz*sizeof(double));
    #pragma omp single nowait
    b = (double *) malloc(sz*sizeof(double));
    #pragma omp barrier

    #pragma omp for nowait
    for ( int ix = 0; ix < sz; ++ix )
      a[ix] = rand_double();

    #pragma omp for nowait
    for ( int ix = 0; ix < sz; ++ix )
      b[ix] = rand_double();
    #pragma omp barrier

    sum = 0.;
    #pragma omp for reduction(+:sum)
    for ( int ix = 0; ix < sz; ++ix )
      sum += a[ix] * b[ix];

    #pragma omp single nowait
    free(a);
    #pragma omp single nowait
    free(b);
  }

  { volatile double tmp = sum;
  }

  return 0;
}
