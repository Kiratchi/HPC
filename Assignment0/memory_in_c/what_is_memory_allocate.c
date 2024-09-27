/*
 * - Arrays are described at
 *   https://en.cppreference.com/w/c/language/array
 * - Memory allocation is described at
 *   https://en.cppreference.com/w/c/memory
 */

#include <stdio.h>
#include <stdlib.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // Memory can be allocated through arrays. Here
  // both allocation and freeing are implicite.

  // Arrays (when declared inside of functions) are
  // allocated on the "stack", a particularly
  // faster spot in memory.
  {
    int a[10];
    for ( int ix = 0; ix < 10; ++ix )
      a[ix] = 15 + 2*ix;
  
    for ( int ix = 0; ix < 9; ++ix )
      printf("%i ", a[ix]);
    printf("%i\n", a[9]);
  }

  // Memory can be allocated through calls to malloc and related functions.
  // Here both allocation and freeing are explicit.

  // This type of memory allocation happens on the "heap", which is slower than
  // the stack.
  {
    int *a = (int*) malloc(10*sizeof(int));
    if ( a == NULL )
      return -1;

    for ( int ix = 0; ix < 10; ++ix )
      a[ix] = -6 + 2*ix;

    for ( int ix = 0; ix < 9; ++ix )
      printf("%u ", a[ix]);
    printf("%u\n", a[9]);

    free(a);
  }

  return 0;
}
