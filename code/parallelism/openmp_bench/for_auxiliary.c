#include <omp.h>

double
sum(
    const double *a,
    const int sz
    )
{
  double s = 0.;
  for ( int ix = 0; ix < sz; ++ix )
    s += a[ix];

  return s;
}

double
sum_openmp(
    const double *a,
    const int sz
    )
{
  omp_set_num_threads(8);

  double s = 0.;
  #pragma omp parallel for reduction(+:s)
  for ( int ix = 0; ix < sz; ++ix )
    s += a[ix];

  return s;
}

double
sum_ij_nocollapse(
    const int sz
    )
{
  omp_set_num_threads(8);

  double s = 0.;
  #pragma omp parallel for reduction(+:s)
  for ( int ix = 0; ix < 4; ++ix )
    for ( int jx = 0; jx < sz; ++jx )
      s += ix*jx;

  return s;
}

double
sum_ij_collapse(
    const int sz
    )
{
  omp_set_num_threads(8);

  double s = 0.;
  #pragma omp parallel for collapse(2) reduction(+:s)
  for ( int ix = 0; ix < 4; ++ix )
    for ( int jx = 0; jx < sz; ++jx )
      s += ix*jx;

  return s;
}

void
matmul_outer(
    double *a,
    const double *const * m,
    const double *b,
    const int sz
    )
{
  omp_set_num_threads(8);

  #pragma omp parallel for
  for (int ix = 0; ix < sz; ++ix) {
    a[ix] = 0.;
    for (int jx = 0; jx < sz; ++jx)
      a[ix] += m[ix][jx] * b[jx];
  }
}

void
matmul_inner(
    double *a,
    const double *const * m,
    const double *b,
    const int sz
    )
{
  omp_set_num_threads(8);

  for (int ix = 0; ix < sz; ++ix) {
    a[ix] = 0.;
    #pragma omp parallel for
    for (int jx = 0; jx < sz; ++jx)
      a[ix] += m[ix][jx] * b[jx];
  }
}
