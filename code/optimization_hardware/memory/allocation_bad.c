#include <stdio.h>
#include <stdlib.h>

void
single_pointer(
    int *as
    )
{
}

void
double_pointer(
    int **as
    )
{
}

void
array_pointer(
    const int sz,
    int (*as)[sz]
    );

int
main(
    int argc,
    char *argv[]
    )
{
  {
    // In 6.2.5:20 it is defined that an array type describes a contiguously
    // allocated nonempty set of objects (as opposed to a part of it, i.e. a
    // contiguous sequence of such objects). The phrase "contiguously
    // allocated" is used nowhere else in the standard, which rather use
    // "contiguous sequence" of objects.
    //
    // When inspecting the definition of the indirection operator it becomes
    // clear that a multi-dimensional array is viewed as an inductive,
    // overlapping structure of arrays. Hence in their allocation, each
    // allocation of the subarrays is viewed as a an allocation itself,
    // matching the requirements of 6.2.5:20.
    //
    // In 6.3.2.3 it is stated that void* can be converted to (object type)*.
    // Arrays types of specified size are complete object types.
    //
    // In 6.5.3.2 it is stated that the indirection operator applied to an
    // invalid (pointer) value is undefined.
    int (*as)[3] = malloc(3*4*sizeof(int));
    // Objects of array type are not modifyable lvalued by 6.3.2.1
    // int as[3][4] = malloc(3*4*sizeof(int));

    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        as[ix][jx] = 10*ix + jx;

    array_pointer(3,as);
    // Both alternative function calls yield conversions of incompatible pointer types.
    // single_pointer(as);
    // double_pointer(as);

    printf("access where declared:\n");
    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        printf("%i ", as[ix][jx]);
    printf("\n");

    free(as);
  }
  
  return 0;
}
