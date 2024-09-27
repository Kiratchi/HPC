static const int bench_iter = 1000;


void
addmul_subcall(
    double *restrict r,
    const double *restrict a,
    const double *restrict b,
    const double *restrict c,
    const int sz
    );

void
addadd_subcall(
    double *restrict r,
    const double *restrict a,
    const double *restrict b,
    const double *restrict c,
    const int sz
    );


void
addmul(
    double *restrict r,
    const double *restrict a,
    const double *restrict b,
    const double *restrict c,
    const int sz
    )
{
  for ( int bx = 0; bx < bench_iter; ++bx ) {
    for ( int ix = 0; ix < sz; ++ix )
      r[ix] = a[ix] * b[ix] + c[ix];
    addmul_subcall(r, a, b, c, sz);
  }
}

void
addadd(
    double *restrict r,
    const double *restrict a,
    const double *restrict b,
    const double *restrict c,
    const int sz
    )
{
  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < sz; ++ix )
      r[ix] = a[ix] + b[ix] + c[ix];
  addadd_subcall(r, a, b, c, sz);
}
