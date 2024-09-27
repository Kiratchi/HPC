#include <stdlib.h>
#include <stdio.h>


int
main(
    int argc,
    char *argv[]
    )
{
  const int sz = 10;
  int *as = (int*) malloc(sizeof(int)*sz);

  for ( int ix = 0; ix < sz; ++ix )
    as[ix] = (int) ix;

  // Two pointer variables in C can refer to the same block of memory.
  // This is called aliasing. The compiler cannot infer in all
  // situations that access to memory is independent, and hence
  // introduces extraneous store and load instructions.
  int *bs = as;
  int *cs = as + 1;
  
  printf("       ");
  for (size_t jx = 0; jx < sz; ++jx)
    printf("%02i ", as[jx]);
  printf("\n");

  for (size_t ix = 0; ix < sz-1; ++ix) {
    as[ix+1] = bs[ix] + cs[ix];

    printf("ix=%i:  ", ix);
    for (size_t jx = 0; jx < sz; ++jx)
      printf("%02i ", as[jx]);
    printf("\n");
  }

  // Note that we have allocated the memory to which as points, so we need to
  // free it (once). It is incorrect to free bs or cs.
  free(as);

  return 0;
}
