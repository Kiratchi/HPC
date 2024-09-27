#include <omp.h>

double
dotproduct(
    const double *restrict a,
    const double *restrict b,
    const int sz,
    int r
    )
{
  double sum = 0.;

  #pragma omp parallel num_threads(r)
  {
    #pragma omp for reduction(+:sum)
    for ( int ix = 0; ix < sz; ++ix )
      sum += a[ix] * b[ix];
  }

  return sum;
}

double
dotproduct_noopenmp(
    const double *restrict a,
    const double *restrict b,
    const int sz
    )
{
  double sum = 0.;

  for ( int ix = 0; ix < sz; ++ix )
    sum += a[ix] * b[ix];

  return sum;
}
