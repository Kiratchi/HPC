#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

auto rd = std::default_random_engine();
auto udist = std::uniform_int_distribution<int> (0,1);


template
  <size_t sz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    unsigned int *src0;
    unsigned int *src1;
    unsigned int *dst;

public:

    ArrayFixture<sz>()
    {
      this->src0 = new unsigned int[sz];
      this->src1 = new unsigned int[sz];
      this->dst  = new unsigned int[sz];

      for ( size_t ix = 0; ix < sz; ++ix ) {
        src0[ix] = udist(rd);
        src1[ix] = udist(rd);
        dst[ix] = 0;
      }
    }

    ~ArrayFixture<sz>()
    {
      delete[] this->src0;
      delete[] this->src1;
      delete[] this->dst;
    }
};


CELERO_MAIN


constexpr size_t sz = 1000000;
typedef ArrayFixture<sz> Fixture;


BASELINE_F(branchless, copy, Fixture, nmbsmpl, nmbiter)
{
  for ( int ix = 0; ix < sz; ++ix ) {
    dst[ix] = src0[ix];
  }
}

BENCHMARK_F(branchless, max_branch, Fixture, nmbsmpl, nmbiter)
{
  for ( int ix = 0; ix < sz; ++ix ) {
    if ( src0[ix] > src1[ix] )
      dst[ix] = src0[ix];
    else
      dst[ix] = src1[ix];
  }
}

BENCHMARK_F(branchless, max_nobranch, Fixture, nmbsmpl, nmbiter)
{
  for ( int ix = 0; ix < sz; ++ix ) {
    dst[ix] = src0[ix] > src1[ix] ? src0[ix] : src1[ix];
  }
}

/* One example run compiled yields:

|   Experiment    |    Baseline     |  us/Iteration   |
|:---------------:|:---------------:|:---------------:|
| copy            |         1.00000 |       294.08294 |
| max_branch      |         1.69101 |       497.29661 |
| max_nobranch    |         1.52254 |       447.75357 |
 */

/* One example run compiled with -march=native yields:
|   Experiment    |    Baseline     |  us/Iteration   |
|:---------------:|:---------------:|:---------------:|
| copy            |         1.00000 |       302.50843 |
| max_branch      |         0.94877 |       287.01129 |
| max_nobranch    |         0.94949 |       287.22902 |
*/
