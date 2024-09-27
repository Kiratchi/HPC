void
addmul_subcall(
    double *restrict r,
    const double *restrict a,
    const double *restrict b,
    const double *restrict c,
    const int sz
    )
{
  for ( int ix = 0; ix < sz; ++ix )
    r[ix] = a[ix] * b[ix] + c[ix];
}

void
addadd_subcall(
    double *restrict r,
    const double *restrict a,
    const double *restrict b,
    const double *restrict c,
    const int sz
    )
{
  for ( int ix = 0; ix < sz; ++ix )
    r[ix] = a[ix] + b[ix] + c[ix];
}
