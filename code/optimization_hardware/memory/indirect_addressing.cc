#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<float> (-10000.,10000.);


template
  <size_t sz, size_t gp>
class ArrayFixture
    : public celero::TestFixture
{
public:
    float *array;
    float **pts_dense;
    float **pts_gaps;
    size_t *pos_dense;
    size_t *pos_gaps;

public:

    ArrayFixture<sz, gp>()
    {

      array     = new float[sz];
      pts_dense = new float*[sz];
      pts_gaps  = new float*[sz];
      pos_dense = new size_t[sz];
      pos_gaps  = new size_t[sz];

      for ( size_t ix = 0; ix < sz; ++ix )
        array[ix] = udist(rd);

      for ( size_t ix = 0, jx = 0; ix < sz; ++ix, jx += gp ) {
        if ( jx >= sz ) jx -= sz;
        pts_dense[ix] = array + ix;
        pts_gaps[ix] = array + jx;
        pos_dense[ix] = ix;
        pos_gaps[ix] = jx;
      }
    }

    ~ArrayFixture<sz, gp>()
    {
      delete[] array;
      delete[] pts_dense;
      delete[] pts_gaps;
      delete[] pos_dense;
      delete[] pos_gaps;
    }
};

CELERO_MAIN

/* Indirect addressing is the operation of accessing memory, whose address
 * itself is given dynamically, for example through an array of pointers or an
 * array of array indices.
 * Its performance impact arises mostly from possibly resulting eradic memory
 * access patterns.
 */

constexpr size_t sz = 1000000;
typedef ArrayFixture<sz,97> ArrayFixtureSmallGaps;
typedef ArrayFixture<sz,100> ArrayFixtureBigGaps;

BASELINE_F(indirect_addressing, direct, ArrayFixtureBigGaps, nmbsmpl, nmbiter)
{
  auto array = this->array;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
     sum += array[ix];
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(indirect_addressing, pointer_dense, ArrayFixtureBigGaps, nmbsmpl, nmbiter)
{
  auto array = this->array;
  auto pts = this->pts_dense;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
     sum += *pts[ix];
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(indirect_addressing, pointer_gaps, ArrayFixtureBigGaps, nmbsmpl, nmbiter)
{
  auto array = this->array;
  auto pts = this->pts_gaps;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
     sum += *pts[ix];
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(indirect_addressing, index_dense, ArrayFixtureBigGaps, nmbsmpl, nmbiter)
{
  auto array = this->array;
  auto pos = this->pos_dense;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
     sum += array[pos[ix]];
  celero::DoNotOptimizeAway(sum);
}

BENCHMARK_F(indirect_addressing, index_gaps, ArrayFixtureBigGaps, nmbsmpl, nmbiter)
{
  auto array = this->array;
  auto pos = this->pos_gaps;
  float sum = 0.f;
  for ( size_t ix = 0; ix < sz; ++ix )
     sum += array[pos[ix]];
  celero::DoNotOptimizeAway(sum);
}

/* One example run yields:

|   Experiment    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:---------------:|:---------------:|:---------------:|:---------------:|
| direct          |         1.00000 |      1005.60775 |          994.42 |
| pointer_dense   |         1.19547 |      1202.17829 |          831.82 |
| pointer_gaps    |         1.69774 |      1707.26066 |          585.73 |
| index_dense     |         1.19774 |      1204.45316 |          830.25 |
| index_gaps      |         1.71267 |      1722.26995 |          580.63 |
 */
