/*
 * - Random number generation is described at
 *   https://en.cppreference.com/w/c/numeric/random
 * - Time functionality is described at
 *   https://en.cppreference.com/w/c/chrono
 * - The volatile type qualifier is described at
 *   https://en.cppreference.com/w/c/language/volatile
 * - Compiler optimization removing unused variables is described in the item
 *   on dead store elimination at
 *   https://en.wikipedia.org/wiki/Optimizing_compiler
 * - More information about micro-benchmarks can be found at
 *   http://www.agner.org/optimize
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double
rand_double()
{
    return (double)rand()/RAND_MAX * 2. - 1.;
}

int
main(
    int argc,
    char *argv[]
    )
{
  const int sz = 10000;
  const long int bench_iter = 20000;
  struct timespec bench_start_time;
  struct timespec bench_stop_time;
  double bench_diff_time;

  double *a = (double*) malloc(sz*sizeof(double));
  double *b = (double*) malloc(sz*sizeof(double));

  // Since performance of some functions depends on their arugment, it is
  // helpful to randomize variables in a relevant range.
  for ( int ix = 0; ix < sz; ++ix )
    a[ix] = rand_double();


  // This is naive benchmarking, unsuitable for micro-benchmarks, examining
  // individual instructions.

  // We repeat the computation to "heat up the cache and CPU".
  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < sz; ++ix )
      b[ix] = sin(a[ix]);


  timespec_get(&bench_start_time, TIME_UTC);

  // Instead of the timespec_get, one can use the POSIX function:
  //clock_gettime(CLOCK_MONOTONIC, &bench_start_time);

  // By passing the argument CLOCK_PROCESS_CPUTIME_ID, one measures one the CPU
  // time, ignoring delays because of for example IO.
  // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &bench_start_time);
  
  // We repeat the computation to average out some of the random effects.
  for ( int bx = 0; bx < bench_iter; ++bx )
    for ( int ix = 0; ix < sz; ++ix )
      b[ix] = sin(a[ix]);

  timespec_get(&bench_stop_time, TIME_UTC);
  // clock_gettime(CLOCK_MONOTONIC, &bench_stop_time);
  // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &bench_stop_time);

  // There are two common strategies to avoid that the compiler optimizes away
  // unused results.

  // Using volatile informs the compiler that each assignment must be treated
  // as "observable" computation.
  { volatile double tmp;
    for ( int ix = 0; ix < sz; ++ix )
      tmp = b[ix];
  }

  // Usually it suffices to access one element of an array or in an allocated
  // block of memory.
  // { volatile double tmp = b[0];
  // }

  // The use of print statements is a bit easier.
  // fprintf(stdout, "auxiliary print: %f\n", b[0]);

  // Sometimes one prefixes this with an if statement, which in reality always
  // yields false, but cannot be proved unconditionally to do so. This avoids a
  // situation where the compiler removes the if-statement.
  // if ( !clock() )
  //   for ( int ix = 0; ix < sz; ++ix )
  //     fprintf(stdout, "auxiliary print: %f\n", b[ix]);

  free(a);
  free(b);

  bench_diff_time =
      difftime(bench_stop_time.tv_sec, bench_start_time.tv_sec) * 1000000
    + (bench_stop_time.tv_nsec - bench_start_time.tv_nsec) / 1000;
  printf("benchmark time for one iteration: %fmus\n",
      bench_diff_time / bench_iter);

  return 0;
}

/* The following is pseudo-reasoning, which illustrates the danger of
 * interpreting bencharks in an uninformed way:
 *
 * On a laptop with nomial 4GHz Skylake CPU (3.46GHz measured by the Linux
 * kernel) the result was 103 microseconds for 10000 sin computations. This
 * is 10.3 nanoseconds for a single sin computation. At 3.46GHz a cycle is
 * about 0.275 nanoseconds. The computation then took about 37 cycles per sin
 * computation.
 *
 * Checking micro-benchmarks for the Skylake architecure, we find 53-105
 * cycles for the FSIN instruction. The Skylake architecure has 2 ports
 * (pipelines) for floating point operations, so one coulds think that we
 * obtained perfectly consistent measurments. The opposite is true!
 *
 * Inspecting the assembler code, we see that a vectorized software
 * implementation is called. FSIN never gets executed. Moreover, to the best
 * of my knowledge only one the pipelines on Skylake is capeable of sin
 * computations.
 */
