#include <stdio.h>
#include <stdlib.h>


double
rand_double()
{
    return (double)rand()/RAND_MAX * 2. - 1.;
}

void
addmul(
    double * r,
    const double * a,
    const double * b,
    const double * c,
    const int sz
    );

void
addadd(
    double * r,
    const double * a,
    const double * b,
    const double * c,
    const int sz
    );


int
main(
    int argc,
    char *argv[]
    )
{
  const int sz = 100000;

  double *a = (double*) malloc(sz * sizeof(double));
  double *b = (double*) malloc(sz * sizeof(double));
  double *c = (double*) malloc(sz * sizeof(double));
  double *r = (double*) malloc(sz * sizeof(double));

  for ( int ix = 0; ix < sz; ++ix ) {
    a[ix] = rand_double();
    b[ix] = rand_double();
    c[ix] = rand_double();
  }

  addmul(r, a, b, c, sz);
  addadd(r, a, b, c, sz);
  addmul(r, a, b, c, sz);

  free(a);
  free(b);
  free(c);
  free(r);

  return 0;
}
