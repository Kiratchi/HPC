#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

int
main_thrd(
    void *args
    )
{
  if ( args != NULL )
    return 1;

  return 0;
}

int
main()
{
  const int nthrds = 8;

  // On creation, a thread initialized a corresponing structure of type thrd_t.
  // This structure can later be used to wait for the threads termination. Very
  // little control beyond this is possible throught the thrd_t structure.
  thrd_t thrds[nthrds];
  
  for ( int tx = 0; tx < nthrds; ++tx ) {
    int r;
    // When creating a thread we have to provide a thread main function and
    // arguments to it. These arguments are passed via one argument of type
    // void*, i.e., a pointer that may point to any other type.
    if ( tx & 1 )
      r = thrd_create(thrds+tx, main_thrd, (void*)main);
    else
      r = thrd_create(thrds+tx, main_thrd, NULL);
    if ( r != thrd_success ) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
  }

  for ( int tx = 0; tx < nthrds; ++tx ) {
    int r;
    thrd_join(thrds[tx], &r);
    if ( tx & 1 )
      assert( r == 1 );
    else 
      assert( r == 0 );
  }

  return 0;
}
