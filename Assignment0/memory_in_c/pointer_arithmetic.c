/*
 * - Pointers are described at
 *   https://en.cppreference.com/w/c/language/pointer
 * - Arithmetic operators for pointers are described at
 *   https://en.cppreference.com/w/c/language/operator_arithmetic
 */

#include <stdio.h>
#include <stdlib.h>

int
main(
    int argc,
    char *argv[]
    )
{
  int a = 0;
  int *ap = &a;

  // We can access the value pointed to by by a pointer by dereferencing it.
  printf("a = %i\n", *ap);
  
  // We may never access memory that we have not allocated. Dereferencing a
  // pointer access memory that it points to.
  int *invalid_pointer = ap + 1;

  printf("equality of a pointers is %i\n", ap == invalid_pointer-1);

  // But it is ok to use an invalid_pointer pointer and compute from it a valid
  // one.
  printf("a = %i\n", *(invalid_pointer-1));

  int *as = (int*) malloc(10*sizeof(int));
  for ( int ix = 0; ix < 10; ++ix )
    as[ix] = ix;

  // For example by shifting a pointer, we can avoid one addition when the
  // natural index set is different from 0 <= ix < 10.
  int *as_pre = as - 1;
  printf("as = ");
  for ( int ix = 1; ix <= 10; ++ix )
    printf("%i ", as_pre[ix]);
  printf("\n");
  
  // We can split up work happening on a "block of memory" by using shifted
  // pointers.
  int *as_part = as;
  printf("as_part = ");
  for ( int ix = 0; ix < 5; ++ix )
    printf("%i ", as_part[ix]);
  printf("\n");

  as_part = as + 5;
  printf("as_part = ");
  for ( int ix = 0; ix < 5; ++ix )
    printf("%i ", as_part[ix]);
  printf("\n");

  // The difference between pointers is also defined, if they lie in the same
  // continguously allocated block of memory.
  printf("difference between pointers is %zu\n", as_part-as);
  // Taking the difference of two pointers in general is undefined behavior.

  return 0;
}
