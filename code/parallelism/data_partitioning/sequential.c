#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double
rand_double()
{
    return (double)rand()/RAND_MAX;
}

int
main()
{
  const int sz = 1000;
  const int niter = 1000;

  double *mentries;
  double **m;
  double *v;
  double *w;

  mentries = (double*) malloc(sz*sz*sizeof(double));
  m = (double**) malloc(sz*sizeof(double*));
  v = (double*) malloc(sz*sizeof(double));
  w = (double*) malloc(sz*sizeof(double));

  for ( int ix = 0, jx = 0; ix < sz; ++ix, jx += sz )
    m[ix] = mentries + jx;

  srand(clock());
  for ( double *e = mentries, *end = mentries + sz*sz; e != end; ++e )
    *e = rand_double();
  for ( double *e = v, *end = v + sz; e != end; ++e )
    *e = rand_double();


  double norm = 1.;
  for ( int lx = 0; lx < niter; ++lx ) {
    // Normalize the vector v to avoid numerical instability.
    for ( int ix = 0; ix < sz; ++ix )
      v[ix] /= norm;

    norm = 0.;
    for ( int ix = 0; ix < sz; ++ix ) {
      const double *mix = m[ix];
      double wix = 0.;
      for ( int kx = 0; kx < sz; ++kx )
        wix += mix[kx] * v[kx];
      w[ix] = wix;
      norm += wix > 0 ? wix : -wix;
    }

    // Swap pointers v and w.
    { double *tmp = v;
      v = w;
      w = tmp;
    }
  }


  for ( int ix = 0; ix < 10; ++ix )
    printf("%f ", v[ix] / w[ix]);
  printf("\n");

  free(m);
  free(mentries);
  free(v);
  free(w);

  return 0;
}
