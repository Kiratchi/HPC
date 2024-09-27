/*
 * - The documentation of GDB can be found at
 *   https://www.gnu.org/software/gdb/documentation/
 * - Assertions are described at
 *   https://en.cppreference.com/w/c/error/assert
 */

#include <assert.h>

void
add_one(
    int *a
    )
{
  // for ( int ix = 0; ix < 6; ++ix )
  for ( int ix = 0; ix < 5; ++ix )
    ++a[ix];
}

void
multiply_by_two(
    int *a
    )
{
  // for ( int ix = 0; ix < 6; ++ix )
  for ( int ix = 1; ix < 6; ++ix )
    a[ix] *= 2;
}

int
main(
    int argc,
    char *argv[]
    )
{
  int a[] = {0, 2, 4, 5, 7, 9};

  add_one(a);
  multiply_by_two(a);
  add_one(a);
  
  assert( a[0] == 3 );
  assert( a[1] == 7 );
  assert( a[2] == 11 );
  assert( a[3] == 13 );
  assert( a[4] == 17 );
  assert( a[5] == 21 );

  return 0;
}
