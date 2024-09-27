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
    int *varconst;
    int *varalt;
    int *varrd;

public:

    ArrayFixture<sz>()
    {
      this->varconst = new int[sz];
      this->varalt = new int[sz];
      this->varrd = new int[sz];

      for ( size_t ix = 0; ix < sz; ++ix ) {
        varconst[ix] = 0;
        varalt[ix] = ix & 1;
        varrd[ix] = udist(rd);
      }
    }

    ~ArrayFixture<sz>()
    {
      delete[] this->varconst;
      delete[] this->varalt;
      delete[] this->varrd;
    }
};


CELERO_MAIN


constexpr size_t sz = 1000000;
typedef ArrayFixture<sz> Fixture;


BASELINE_F(branching, notif, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for ( int ix = 0; ix < sz; ++ix ) {
    celero::DoNotOptimizeAway(a);
  }
}

BENCHMARK_F(branching, constif_aa, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for (int ix = 0; ix < sz; ++ix) {
    if ( varconst[ix] )
      celero::DoNotOptimizeAway(a);
    else
      celero::DoNotOptimizeAway(a);
  }
}

BENCHMARK_F(branching, constif_ab, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for (int ix = 0; ix < sz; ++ix) {
    if ( varconst[ix] )
      celero::DoNotOptimizeAway(a);
    else
      celero::DoNotOptimizeAway(b);
  }
}

BENCHMARK_F(branching, altif_aa, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for (int ix = 0; ix < sz; ++ix) {
    if ( varalt[ix] )
      celero::DoNotOptimizeAway(a);
    else
      celero::DoNotOptimizeAway(a);
  }
}

BENCHMARK_F(branching, altif_ab, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for (int ix = 0; ix < sz; ++ix) {
    if ( varalt[ix] )
      celero::DoNotOptimizeAway(a);
    else
      celero::DoNotOptimizeAway(b);
  }
}

BENCHMARK_F(branching, rdif_aa, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for (int ix = 0; ix < sz; ++ix) {
    if ( varrd[ix] )
      celero::DoNotOptimizeAway(a);
    else
      celero::DoNotOptimizeAway(a);
  }
}

BENCHMARK_F(branching, rdif_ab, Fixture, nmbsmpl, nmbiter)
{
  int a = 0, b = 0;
  for (int ix = 0; ix < sz; ++ix) {
    if ( varrd[ix] )
      celero::DoNotOptimizeAway(a);
    else
      celero::DoNotOptimizeAway(b);
  }
}

/* One example run yields:

|   Experiment    |    Baseline     |  us/Iteration   |
|:---------------:|:---------------:|:---------------:|
| notif           |         1.00000 |       503.23589 |
| constif_aa      |         3.04822 |      1533.97590 |
| constif_ab      |         5.01188 |      2522.15725 |
| altif_aa        |         3.48517 |      1753.86232 |
| altif_ab        |         4.19511 |      2111.13053 |
| rdif_aa         |         3.52198 |      1772.38832 |
| rdif_ab         |        13.10527 |      6595.04110 |
 */
