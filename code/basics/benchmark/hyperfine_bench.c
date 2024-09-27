/*
 * Confer the makefile to see how to install Celero.
 */

#include <math.h>
#include <stdlib.h>

double
rand_double()
{
    return(double)rand()/RAND_MAX * 2. - 1.;
}

int
main(
    int argc,
    char *argv[]
    )
{
  // We use more iterations in order to push the runtime above 5 milliseconds.
  const int sz = 1000000;

  double *a = (double*) malloc(sz*sizeof(double));
  double *b = (double*) malloc(sz*sizeof(double));

  // Since performance of some functions depends on their arugment, it is
  // helpful to randomize variables in a relevant range.
  for ( int ix = 0; ix < sz; ++ix )
    a[ix] = rand_double();

  for ( int ix = 0; ix < sz; ++ix )
    b[ix] = sin(a[ix]);

  // Since hyperfine measures the runtime of the whole program, it is advisable
  // to use the one-variable access only.
  { volatile double tmp = b[0];
  }

  free(a);
  free(b);

  return 0;
}
