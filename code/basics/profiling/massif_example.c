#include <stdio.h>
#include <stdlib.h>


int
main(
    int argc,
    char *argv[]
    )
{
  const int sz = 10000000;

  double *a = (double*) malloc(sz * sizeof(double));
  double *b = (double*) malloc(sz * sizeof(double));
  double *c;
  double *r = (double*) malloc(sz * sizeof(double));

  for ( int bx = 0; bx < 10; ++bx ) {
    c = (double*) malloc(sz * sizeof(double));
    for ( int ix = 0; ix < sz; ++ix )
      r[ix] = a[ix] * b[ix] + c[ix];
    free(c);
  }

  { volatile double tmp = r[0];
    for ( int ix = 0; ix < sz; ++ix )
      tmp = r[ix];
  }

  free(a);
  free(b);
  free(r);

  return 0;
}
