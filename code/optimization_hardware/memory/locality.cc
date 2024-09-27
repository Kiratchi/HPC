#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<float> (-10000.,10000.);


template
  <size_t sz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    float ***array;
    float **rows;
    float *entries;

public:

    ArrayFixture<sz>()
    {
      entries = new float[sz*sz*sz];
      rows    = new float*[sz*sz];
      array   = new float**[sz];

      for ( size_t ix = 0; ix < sz*sz*sz; ++ix )
        entries[ix] = udist(rd);

      auto row = entries;
      for ( size_t ix = 0; ix < sz*sz; ++ix, row += sz )
        rows[ix] = row;

      auto block = rows;
      for ( size_t ix = 0; ix < sz; ++ix, block += sz )
        array[ix] = block;
    }

    ~ArrayFixture<sz>()
    {
      delete[] entries;
      delete[] rows;
      delete[] array;
    }
};

CELERO_MAIN

/* Memory locality refers to the feature (or absense thereof) of a program to
 * preferable access parts in memory whose addess is close to each other.
 */

constexpr size_t sz = 700;
typedef ArrayFixture<sz> Fixture;


// average distance between access is sizeof(float) * 1
// incrementing directly the pointer, avoiding extraneous integer arithmetic
BASELINE_F(locality, linear, Fixture, nmbsmpl, nmbiter)
{
  auto entries = this->entries;
  auto end = entries + sz*sz*sz;
  float sum = 0.f;
  for ( float* pt = entries; pt != end; ++pt )
    sum += *pt;
  celero::DoNotOptimizeAway(sum);
}

// average distance between access is sizeof(float) * 1
BENCHMARK_F(locality, ijk, Fixture, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      for ( size_t kx = 0; kx < sz; ++kx )
        sum += array[ix][jx][kx];
  celero::DoNotOptimizeAway(sum);
}

// average distance between access is sizeof(float) * 17.0
BENCHMARK_F(locality, jik, Fixture, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      for ( size_t kx = 0; kx < sz; ++kx )
        sum += array[jx][ix][kx];
  celero::DoNotOptimizeAway(sum);
}

// average distance between access is sizeof(float) * 17.2
BENCHMARK_F(locality, ikj, Fixture, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      for ( size_t kx = 0; kx < sz; ++kx )
        sum += array[ix][kx][jx];
  celero::DoNotOptimizeAway(sum);
}

// average distance between access is sizeof(float) * 17.8
BENCHMARK_F(locality, jki, Fixture, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      for ( size_t kx = 0; kx < sz; ++kx )
        sum += array[jx][kx][ix];
  celero::DoNotOptimizeAway(sum);
}

// average distance between access is sizeof(float) * 179.2
BENCHMARK_F(locality, kij, Fixture, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      for ( size_t kx = 0; kx < sz; ++kx )
        sum += array[kx][ix][jx];
  celero::DoNotOptimizeAway(sum);
}

// average distance between access is sizeof(float) * 179.2
BENCHMARK_F(locality, kji, Fixture, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      for ( size_t kx = 0; kx < sz; ++kx )
        sum += array[kx][jx][ix];
  celero::DoNotOptimizeAway(sum);
}

/* One example run with sz = 200 yields:

|   Experiment    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:---------------:|:---------------:|:---------------:|:---------------:|
| linear          |         1.00000 |      8904.95495 |          112.30 |
| ijk             |         1.11100 |      9893.39394 |          101.08 |
| jik             |         1.05075 |      9356.87368 |          106.87 |
| ikj             |         1.43965 |     12820.05618 |           78.00 |
| jki             |         3.62246 |     32257.82353 |           31.00 |
| kij             |         1.61850 |     14412.70370 |           69.38 |
| kji             |         2.77554 |     24716.08696 |           40.46 |


   One example run with sz = 700 yields:

|   Experiment    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:---------------:|:---------------:|:---------------:|:---------------:|
| linear          |         1.00000 |    384103.00000 |            2.60 |
| ijk             |         0.99967 |    383978.00000 |            2.60 |
| jik             |         1.08332 |    416105.50000 |            2.40 |
| ikj             |         1.66870 |    640952.00000 |            1.56 |
| jki             |        18.20361 |   6992060.00000 |            0.14 |
| kij             |         2.88575 |   1108426.00000 |            0.90 |
| kji             |        33.47760 |  12858846.00000 |            0.08 |
 */
