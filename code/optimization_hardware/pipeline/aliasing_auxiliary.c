static const double s_inc = 1e-8;

void
addmul_pointer(
    double *a,
    const double *b,
    const double *c,
    double *s,
    int sz
  )
{
  for (int ix = 0; ix < sz; ++ix) {
    a[ix] = b[ix] + *s * c[ix];
    *s += s_inc;
  }
}

void
addmul_local_copy(
    double *a,
    const double *b,
    const double *c,
    double *s,
    int sz
  )
{
  double s_tmp = *s;
  for (int ix = 0; ix < sz; ++ix) {
    a[ix] = b[ix] + s_tmp * c[ix];
    s_tmp += s_inc;
  }
  *s = s_tmp;
}

void
addmul_restrict(
    double *restrict a,
    const double *restrict b,
    const double *restrict c,
    double *restrict s,
    int sz
  )
{
  for (int ix = 0; ix < sz; ++ix) {
    a[ix] = b[ix] + *s * c[ix];
    *s += s_inc;
  }
}
