#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

int
main_thrd(
    void *args
    )
{
  double a = 1e4;
  double b = 1e-1;
  for ( int ix = 0; ix < 100000; ++ix )
    a -= b;

  *((double*)args) = a;

  return 0;
}

int
main()
{
  const int nthrds = 4;

  thrd_t thrds[nthrds];
  double as[nthrds];
  
  for ( int tx = 0; tx < nthrds; ++tx ) {
    int r;
    r = thrd_create(thrds+tx, main_thrd, as+tx);
    if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
  }

  for ( int tx = 0; tx < nthrds; ++tx )
    thrd_join(thrds[tx], NULL);

  for ( int tx = 0; tx < nthrds; ++tx )
    printf("a = %lf\n", as[tx]);

  return 0;
}
