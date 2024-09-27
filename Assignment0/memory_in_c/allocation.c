/*
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
  {
    // There is two ways of dynamically allocating a two-dimensional "array".
    // The first one is closest to actual arrays, in so-far that it employs a
    // contiguous.
    // There is a unique way to ensure that memory is contiguous: By allocating
    // it through a single call to malloc/calloc/realloc/aligned_alloc.
    int *asentries = (int*) malloc(3*4*sizeof(int));

    // A two-dimensional array, when allocated dynamically, corresponds to
    // pointers to pointers that point to the memory encoding the matrix rows.
    int **as = (int**) malloc(3*sizeof(int*));
    for ( int ix = 0; ix < 3; ++ix )
      // By adding to a pointer by x we shift the address by as much as it needs
      // to represent x instances of the underlying type.
      as[ix] = asentries + 4*ix;

    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        as[ix][jx] = 10*ix + jx;

    // Since the memory underying asentries is contiguous, we can access it in a
    // liear way, exactly as in the case of arrarys.
    printf("Contiguous memory underying two-dimensional array:\n");
    for ( int ijx = 0; ijx < 3*4; ++ijx )
      printf("%i ", asentries[ijx]);
    printf("\n");

    // Each allocation should correspond to one free.
    free(as);
    free(asentries);
  }

  printf("\n");

  // When allocating "arrays", it is also possible to allocate memory for each
  // row separately. This way it is no longer possible to ensure that this
  // memory is contiguous, but we have the flexibility of freeing rows
  // independently of each other.
  {
    int **as = (int**) malloc(3*sizeof(int*));
    for ( int ix = 0; ix < 3; ++ix )
      as[ix] = (int*) malloc(4*sizeof(int));

    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        as[ix][jx] = 10*ix + jx;

    printf("Fragmented two-dimensional array:\n");
    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        printf("%i ", as[ix][jx]);
    printf("\n");

    // As before, each allocation should correspond to one free.
    for ( int ix = 0; ix < 3; ++ix )
      free(as[ix]);
    free(as);
  }

  printf("\n");

  // The flexibility offered by fragmented memory can be taken to the extrem:
  // allocate just before use and free right afterwards.
  {
    int **as = (int**) malloc(3*sizeof(int*));

    printf("Fragmented two-dimensional array with tight memory use:\n");
    for ( int ix = 0; ix < 3; ++ix ) {
      as[ix] = (int*) malloc(4*sizeof(int));

      for ( int jx = 0; jx < 4; ++jx )
        as[ix][jx] = 10*ix + jx;

      for ( int jx = 0; jx < 4; ++jx )
        printf("%i ", as[ix][jx]);

      free(as[ix]);
    }

    free(as);

    printf("\n");
  }

  printf("\n");

  // Asking for the address of usual variables forces the system to move them
  // into the "adress space" as opposed to alternatives that are faster. These
  // addresses are on the stack.
  {
    int a = 2, b = 3;
    int as[] = {2, 3};
    int *bs = (int*) malloc(sizeof(int));
    
    printf("address of a is %p\n", &a);
    printf("address of b is %p\n", &b);
    printf("address of as is %p\n", as);
    printf("address of bs is %p\n", bs);
  }
  
  return 0;
}
