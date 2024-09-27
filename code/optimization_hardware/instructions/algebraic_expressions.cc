#include <random>
#include "celero/Celero.h"


// The value zero instructs Celero to make an automatic choice.
constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();
auto udist =
  std::uniform_real_distribution<double>(-10000.,10000.);


CELERO_MAIN

BASELINE(algebraic_expressions, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist(rd));
}

BENCHMARK(algebraic_expressions, pow, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  celero::DoNotOptimizeAway(pow(sqrt(a), 3));
}

BENCHMARK(algebraic_expressions, mulbbb, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  auto b = sqrt(a);
  celero::DoNotOptimizeAway(b*b*b);
}

BENCHMARK(algebraic_expressions, mulab, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  auto b = sqrt(a);
  celero::DoNotOptimizeAway(a*b);
}

/* One example run yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|algebraic_expre | nullop          |         1.00000 |         0.01073 |
|algebraic_expre | pow             |         3.34993 |         0.03593 |
|algebraic_expre | mulbbb          |         2.13371 |         0.02289 |
|algebraic_expre | mulab           |         2.13869 |         0.02294 |
*/
