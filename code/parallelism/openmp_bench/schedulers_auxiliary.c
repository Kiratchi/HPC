void
triangular_sum(
    double * a,
    const double * b,
    const int sz
    )
{
  for ( int ix = 0; ix < sz; ++ix ) {
    a[ix] = 0.;
    for ( int jx = ix; jx < sz; ++jx )
      a[ix] += 2*b[jx];
  }
}

void
triangular_sum_static(
    double * a,
    const double * b,
    const int sz,
    const int chksz
    )
{
  #pragma omp parallel for schedule(static, chksz)
  for ( int ix = 0; ix < sz; ++ix ) {
    a[ix] = 0.;
    for ( int jx = ix; jx < sz; ++jx )
      a[ix] += 2*b[jx];
  }
}

void
triangular_sum_dynamic(
    double * a,
    const double * b,
    const int sz,
    const int chksz
    )
{
  #pragma omp parallel for schedule(dynamic, chksz)
  for ( int ix = 0; ix < sz; ++ix ) {
    a[ix] = 0.;
    for ( int jx = ix; jx < sz; ++jx )
      a[ix] += 2*b[jx];
  }
}

void
triangular_sum_guided(
    double * a,
    const double * b,
    const int sz
    )
{
  #pragma omp parallel for schedule(guided)
  for ( int ix = 0; ix < sz; ++ix ) {
    a[ix] = 0.;
    for ( int jx = ix; jx < sz; ++jx )
      a[ix] += 2*b[jx];
  }
}
