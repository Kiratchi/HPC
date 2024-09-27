#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

void
add(
    double *a,
    const double b,
    const double c)
{
  *a = b + c;
}

int
main()
{
  const int sz = 1000;
  double *v = (double*) malloc(sz*sizeof(double));
  double *w = (double*) malloc(sz*sizeof(double));
  double sums[4];
  double sum;

  for ( int ix = 0; ix < sz; ++ix )
    v[ix] = ix+1;

  // The number of threads used by default heavily depends on the
  // circumstances.
  omp_set_num_threads(4);

  
  printf("shared private\n");

  w[0] = 0.;
  #pragma omp parallel for
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  w[0] = 0.;
  #pragma omp parallel for shared(v,w)
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  w[0] = 0.;
  #pragma omp parallel for default(none) shared(v,w)
  for ( int ix = 0; ix < sz; ++ix )
    w[ix] = 2*v[ix];

  printf("w[0] = %f\n", w[0]);


  w[0] = 0.;
  {
    int ix;
    #pragma omp parallel for shared(v,w) private(ix)
    for ( ix = 0; ix < sz; ++ix )
      w[ix] = 2*v[ix];
  }

  printf("w[0] = %f\n", w[0]);


  // Beware that the private attribute can be misleading for pointers. The
  // following code might segfault, since there is no guarantee on what w
  // points to.
  /*
   * w[0] = 0.;
   * #pragma omp parallel for shared(v) private(w)
   * for ( int ix = 0; ix < sz; ++ix )
   *   w[ix] = 2*v[ix];

   * printf("w[0] = %f\n", w[0]);
   */


  printf("\nfirstprivate lastprivate\n");

  // firstprivate initializes variables, but may not be of pointer type and may
  // not be constant.
  w[0] = 0.;

  {
    int scale = 2;
    #pragma omp parallel firstprivate(scale)
    {
      #pragma omp for 
      for ( int ix = 0; ix < sz; ++ix )
        w[ix] = scale*v[ix];
      scale = 0;
    }
    printf("scale is %i  ", scale);
  }

  printf("w[0] = %f\n", w[0]);


  // lastprivate is the analogue of copyprivate seen in the single construct.
  // It is not associated with the parallel construct but with for (or
  // sections).
  w[0] = 0.;
  {
    int ix;
    #pragma omp parallel for lastprivate(ix)
    for ( ix = 0; ix < sz; ++ix )
      w[ix] = 2*v[ix];
    printf("ix = %i  ", ix);
  }

  printf("w[0] = %f\n", w[0]);


  printf("\nfunction calls\n");

  // When calling a function inside a parallel environment, then references
  // (pointers) inherit attributes and values become private.
  {
    double a[omp_get_max_threads()];
    double b;
    b = v[1];
    #pragma omp parallel shared(v,a) private(b)
    {
      add(a + omp_get_thread_num(), b, v[0]);
    }

    printf("a[0] = %f\n", a[0]);
  }


  // reduction and copyprivate are also attributes.


  free(v);
  free(w);

  return 0;
}
