#include <stdio.h>
#include <math.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // For CPU bound applications it is typical to feature costly
  // instructions executed on little data.

  for ( int ix = 0; ix < (1<<27); ++ix ) {
    double a = (double) ix;
    double b = sin(a)*cos(a) + sin(a)*cos(a);
    if ( b > 10. )
      printf("this can never happen\n");
  }

  return 0;
}
