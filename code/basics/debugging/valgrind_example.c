/*
 * - The documentation of valgrind can be found at
 *   https://www.valgrind.org/docs/
 * - The keyword volatile is described at
 *   https://en.cppreference.com/w/c/language/volatile
 */

#include <stdlib.h>

int
main(
    int argc,
    char *argv[]
    )
{
  volatile int *a = (int*) malloc(7*sizeof(int));

  // for ( int ix = 0; ix < 7; ++ix )
  for ( int ix = 0; ix < 8; ++ix )
    a[ix] = 2;

  // free((void*) a);

  return 0;
}
