#include <random>
#include "celero/Celero.h"


// The value zero instructs Celero to make an automatic choice.
constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();
auto udist =
  std::uniform_real_distribution<double>(-10000.,10000.);


CELERO_MAIN

BASELINE(operations_rand, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist(rd));
  celero::DoNotOptimizeAway(udist(rd));
}

BENCHMARK(operations_rand, add, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  auto b = udist(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(operations_rand, mul, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  auto b = udist(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(operations_rand, div, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  auto b = udist(rd);
  celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(operations_rand, sqrt, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  celero::DoNotOptimizeAway(udist(rd));
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(operations_rand, cos, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  celero::DoNotOptimizeAway(udist(rd));
  celero::DoNotOptimizeAway(cos(a));
}

BENCHMARK(operations_rand, exp, nmbsmpl, nmbiter)
{
  auto a = udist(rd);
  celero::DoNotOptimizeAway(udist(rd));
  celero::DoNotOptimizeAway(exp(a));
}

auto a = udist(rd);
auto b = udist(rd);

BASELINE(operations_fixed, nullop, nmbsmpl, nmbiter)
{
}

BENCHMARK(operations_fixed, add, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(operations_fixed, mul, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(operations_fixed, div, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(operations_fixed, sqrt, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(operations_fixed, cos, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(cos(a));
}

BENCHMARK(operations_fixed, exp, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(exp(a));
}

/* One example run yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|operations_rand | nullop          |         1.00000 |         0.01697 |
|operations_fixe | nullop          |         1.00000 |         0.00478 |
|operations_rand | add             |         0.97302 |         0.01651 |
|operations_rand | mul             |         0.97302 |         0.01651 |
|operations_rand | div             |         1.00315 |         0.01702 |
|operations_rand | sqrt            |         1.54676 |         0.02625 |
|operations_rand | cos             |         2.22662 |         0.03778 |
|operations_rand | exp             |         1.37725 |         0.02337 |
|operations_fixe | add             |         1.28663 |         0.00615 |
|operations_fixe | mul             |         1.28942 |         0.00616 |
|operations_fixe | div             |         1.27425 |         0.00609 |
|operations_fixe | sqrt            |         2.34331 |         0.01120 |
|operations_fixe | cos             |         3.18802 |         0.01523 |
|operations_fixe | exp             |         2.35130 |         0.01123 |
*/
