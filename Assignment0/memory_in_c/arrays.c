/*
 * - Arrays are described at
 *   https://en.cppreference.com/w/c/language/array
 * - Pseudo-random number generation is described at
 *   https://en.cppreference.com/w/c/numeric/random
 * - An overview of date and time functions is given at
 *   https://en.cppreference.com/w/c/chrono
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // Pseudo-random number generators require
  // initialization. In domains that are not relevant
  // for security, initialization by time is a common
  // pattern.
  srand(time(NULL));

  // When declaring arrays, like all other variables,
  // they will not be initialized. This is sometimes
  // a bit hard to observe, since the operating
  // system clears part of the initial memory
  // assigned to the program.

  for ( int ix = 0; ix < 10; ++ix ) {
    int as[10];
    printf("Values in iteration %i:\n", ix);
    for ( int jx = 0; jx < 9; ++jx )
      printf("%i ", as[jx]);
    printf("%i\n", as[9]);

    for ( int jx = 0; jx < 10; ++jx )
      as[jx] = rand();
  }

  printf("\n");

  // Multi-dimensional arrays are essentially syntactic sugar for longer
  // one-dimensional ones.

  // When taking the address of an array and iterating through the elements
  // in linear order, we see that indix step sizes get small from left to
  // right.
  {
    int as[3][4];
    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        as[ix][jx] = 10*ix + jx;

    printf("Linearized two-dimensional array:\n");
    int *aspointer = (int*) as; 
    for ( int ijx = 0; ijx < 3*4; ++ijx )
      printf("%i ", aspointer[ijx]);
    printf("\n");
  }

  printf("\n");

  // The interpretation of arrays as one contiguous places in memory allows us
  // to also access subarrays.
  {
    int as[3][4][5];
    for ( int ix = 0; ix < 3; ++ix )
      for ( int jx = 0; jx < 4; ++jx )
        for ( int kx = 0; kx < 5; ++kx )
          as[ix][jx][kx] = 100*ix + 10*jx + kx;

    for ( int ix = 0; ix < 3; ++ix ) {
      printf("Block %i (subarray) of multi-dimensional array:\n", ix);

      // int *bs[4][5] would be an array of pointers.
      // int bs[4][5] = as[ix]; on first sight seems to make sense, but is an
      // illegal array initialization.
      int (*bs)[4][5] = &as[ix]; 
      for ( int jx = 0; jx < 4; ++jx )
        for ( int kx = 0; kx < 5; ++kx )
          printf("%i ", (*bs)[jx][kx]);
      printf("\n");

      printf("Linearized subarray:\n");
      int *bsp = (int*) as[ix]; 
      for ( int jkx = 0; jkx < 4*5; ++jkx )
        printf("%i ", bsp[jkx]);
      printf("\n");

      if ( ix != 3-1)
        printf("\n");
    }
  }

  printf("\n");

  // It is often conventient to use array initialization.
  {
    int a = 2;
    int bs[] = {1, 2};
    int cs[][2] = {{1, 2}, {3, 4}};

    printf("Array initialized variables:\n");
    printf("a = %i\n", a);
    printf("as = %i %i\n", bs[0], bs[1]);
    printf("cs = %i %i %i %i\n", cs[0][0], cs[0][1], cs[1][0], cs[1][1]);
  }

  return 0;
}
