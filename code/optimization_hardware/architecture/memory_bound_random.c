#include <stdio.h>
#include <stdlib.h>

int
main(
    int argc,
    char *argv[]
    )
{
  // A variant of memory bound applications access data in irregular patterns.

  const int sz = 1<<29;
  const int pz = 1<<24; // one 32-th of sz
  const int pstep = 1 << 20;
  const unsigned int pmask = sz-1;

  int *p = (int*) malloc(pz*sizeof(int));
  for ( int ix = 0, jx = 0; ix < pz; ++ix, jx += pstep, jx &= pmask )
    p[ix] = jx;


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
  
    for ( int ix = 0; ix < pz; ++ix ) {
      a[p[ix]] = ix + 2;
      b[p[ix]] = 2*ix + 3;
    }
  
    { volatile int tmp = a[sz-1];
      tmp = b[sz-1];
    }
  
    for ( int ix = 0; ix < pz; ++ix )
      a[p[ix]] += 3*b[p[ix]];
  
  
    { volatile int tmp = a[sz-1];
    }
  
    free(a);
    free(b);
  }

  return 0;
}
