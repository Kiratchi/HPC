#include <stdio.h>
#include <stdlib.h>

// In 6.7.6.3 it is explicitely stated that the array type need to be change
// to pointer type. Notice also Example 5, which explicitely states
// equivalence with a variable length array type.
// 
void
array_pointer(
    const int sz,
    int (*as)[sz]
    )
{
  printf("access through function\n");
  for ( int ix = 0; ix < 3; ++ix )
    for ( int jx = 0; jx < 4; ++jx )
      printf("%i ", as[ix][jx]);
  printf("\n");
}
