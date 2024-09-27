/*
 * Confer the makefile to see how to install Celero.
 */

#include <cstring>
#include "celero/Celero.h"

constexpr size_t szs = 50;
constexpr size_t szb = 1000;

constexpr size_t nmbsmpl = 100;
constexpr size_t nmbiter = 1000000;

CELERO_MAIN

BASELINE(memcpy, baseline, nmbsmpl, nmbiter)
{
  int * a = (int*) malloc(sizeof(int)*2*szs);
  celero::DoNotOptimizeAway(a);
  free(a);
}

BENCHMARK(memcpy, direct_access_small, nmbsmpl, nmbiter)
{
  int * a = (int*) malloc(sizeof(int)*2*szs);
  for ( size_t ix = 0; ix < szs; ++ix )
    a[ix+szs] = a[ix];
  celero::DoNotOptimizeAway(a);
  free(a);
}

BENCHMARK(memcpy, memcpy_small, nmbsmpl, nmbiter)
{
  int * a = (int*) malloc(sizeof(int)*2*szs);
  memcpy(a, a+szs, sizeof(int)*szs);
  celero::DoNotOptimizeAway(a);
  free(a);
}

BENCHMARK(memcpy, direct_access_big, nmbsmpl, nmbiter)
{
  int * a = (int*) malloc(sizeof(int)*2*szb);
  for ( size_t ix = 0; ix < szb; ++ix )
    a[ix+szb] = a[ix];
  celero::DoNotOptimizeAway(a);
  free(a);
}

BENCHMARK(memcpy, memcpy_big, nmbsmpl, nmbiter/100)
{
  int * a = (int*) malloc(sizeof(int)*2*szb);
  memcpy(a, a+szb, sizeof(int)*szb);
  celero::DoNotOptimizeAway(a);
  free(a);
}

/* One example run of the benchmark compiled with -O1 yields:

Timer resolution: 0.001000 us
|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|memcpy          | baseline        |         1.00000 |         0.00564 |
|memcpy          | direct_access_s |         4.01294 |         0.02264 |
|memcpy          | memcpy_small    |         1.01081 |         0.00570 |
|memcpy          | direct_access_b |        55.65314 |         0.31400 |
|memcpy          | memcpy_big      |         0.99256 |         0.00560 |

* When compiled with -O2 the benchmark looks very different:

Timer resolution: 0.001000 us
|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|memcpy          | baseline        |         1.00000 |         0.00566 |
|memcpy          | direct_access_s |         1.02472 |         0.00580 |
|memcpy          | memcpy_small    |         1.02489 |         0.00581 |
|memcpy          | direct_access_b |         1.02419 |         0.00580 |
|memcpy          | memcpy_big      |         0.98870 |         0.00560 |

*/
