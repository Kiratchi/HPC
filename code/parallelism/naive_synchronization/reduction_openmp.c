#include <omp.h>
#include <stdlib.h>
#include <stdio.h>


int
main()
{
  const int sz = 5000000;
  long int *v = (long int*) malloc(sz*sizeof(long int));

  for ( int ix = 0; ix < sz; ++ix )
    v[ix] = ix+1;

  omp_set_num_threads(8);


  long int sum = 0;
  #pragma omp parallel for shared(v) reduction(+:sum)
  for ( int ix = 0; ix < sz; ++ix )
    sum += v[ix];

  printf("difference: %li\n", sum - sz * (long int)(sz + 1) / 2);

  return 0;
}
