#include <stdio.h>
#include <stdlib.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // For memory bound applications it is typical to feature simple
  // instructions that need to be executed on a lot of data.

  const int sz = 1<<29;

  for ( int tx = 0; tx < 5; ++tx ) {
    int *a = (int*) malloc(sz*sizeof(int)); // 1 GB of data
    if ( a == NULL ) {
      fprintf(stderr, "failed allocating memory\n");
      return -1;
    }
  
    int *b = (int*) malloc(sz*sizeof(int)); // 1 GB of data
    if ( b == NULL ) {
      fprintf(stderr, "failed allocating memory\n");
      free(a);
      return -1;
    }
  
    for ( int ix = 0; ix < sz; ++ix ) {
      a[ix] = ix + 2;
      b[ix] = 2*ix + 3;
    }
  
    { volatile int tmp = a[sz-1];
      tmp = b[sz-1];
    }
  
    for ( int ix = 0; ix < sz; ++ix )
      a[ix] += 3*b[ix];
  
  
    { volatile int tmp = a[0];
    }
  
    free(a);
    free(b);
  }

  return 0;
}
