#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();
auto udist_double_small =
  std::uniform_real_distribution<double> (-1e2,1e2);
auto udist_double_large =
  std::uniform_real_distribution<double> (-1e15,1e15);
auto udist_double_pos_small =
  std::uniform_real_distribution<double> (0,1e2);
auto udist_double_pos_large =
  std::uniform_real_distribution<double> (0,1e15);
auto udist_long_small =
  std::uniform_int_distribution<long int>(-100,100);
auto udist_long_large =
  std::uniform_int_distribution<long int>(
    std::numeric_limits<long int>::min(),
    std::numeric_limits<long int>::max());


CELERO_MAIN

////////////////////////////////////////////////////////////
// float
////////////////////////////////////////////////////////////

BASELINE(values_float, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_double_large(rd));
  celero::DoNotOptimizeAway(udist_double_large(rd));
}

BENCHMARK(values_float, add_small, nmbsmpl, nmbiter)
{
  auto a = udist_double_small(rd);
  auto b = udist_double_small(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(values_float, add_large, nmbsmpl, nmbiter)
{
  auto a = udist_double_large(rd);
  auto b = udist_double_large(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(values_float, mul_small, nmbsmpl, nmbiter)
{
  auto a = udist_double_small(rd);
  auto b = udist_double_small(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(values_float, mul_large, nmbsmpl, nmbiter)
{
  auto a = udist_double_large(rd);
  auto b = udist_double_large(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(values_float, div_small, nmbsmpl, nmbiter)
{
  auto a = udist_double_small(rd);
  auto b = udist_double_small(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(values_float, div_large, nmbsmpl, nmbiter)
{
  auto a = udist_double_large(rd);
  auto b = udist_double_large(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(values_float, sqrt_small, nmbsmpl, nmbiter)
{
  auto a = udist_double_pos_small(rd);
  auto b = udist_double_pos_small(rd);
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(values_float, sqrt_large, nmbsmpl, nmbiter)
{
  auto a = udist_double_pos_large(rd);
  auto b = udist_double_pos_large(rd);
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(values_float, sin_small, nmbsmpl, nmbiter)
{
  auto a = udist_double_small(rd);
  auto b = udist_double_small(rd);
  celero::DoNotOptimizeAway(sin(a));
}

BENCHMARK(values_float, sin_large, nmbsmpl, nmbiter)
{
  auto a = udist_double_large(rd);
  auto b = udist_double_large(rd);
  celero::DoNotOptimizeAway(sin(a));
}

BENCHMARK(values_float, exp_small, nmbsmpl, nmbiter)
{
  auto a = udist_double_small(rd);
  auto b = udist_double_small(rd);
  celero::DoNotOptimizeAway(exp(a));
}

BENCHMARK(values_float, exp_large, nmbsmpl, nmbiter)
{
  auto a = udist_double_large(rd);
  auto b = udist_double_large(rd);
  celero::DoNotOptimizeAway(exp(a));
}

////////////////////////////////////////////////////////////
// int
////////////////////////////////////////////////////////////

BASELINE(values_int, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_long_large(rd));
  celero::DoNotOptimizeAway(udist_long_large(rd));
}

BENCHMARK(values_int, add_small, nmbsmpl, nmbiter)
{
  auto a = udist_long_small(rd);
  auto b = udist_long_small(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(values_int, add_large, nmbsmpl, nmbiter)
{
  auto a = udist_long_large(rd);
  auto b = udist_long_large(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(values_int, mul_small, nmbsmpl, nmbiter)
{
  auto a = udist_long_small(rd);
  auto b = udist_long_small(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(values_int, mul_large, nmbsmpl, nmbiter)
{
  auto a = udist_long_large(rd);
  auto b = udist_long_large(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(values_int, div_small, nmbsmpl, nmbiter)
{
  auto a = udist_long_small(rd);
  auto b = udist_long_small(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(values_int, div_large, nmbsmpl, nmbiter)
{
  auto a = udist_long_large(rd);
  auto b = udist_long_large(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

/* One example run yields:
|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|values_float    | nullop          |         1.00000 |         0.01696 |
|values_int      | nullop          |         1.00000 |         0.07162 |
|values_float    | add_small       |         0.97526 |         0.01654 |
|values_float    | add_large       |         0.97616 |         0.01656 |
|values_float    | mul_small       |         0.97571 |         0.01655 |
|values_float    | mul_large       |         0.97481 |         0.01653 |
|values_float    | div_small       |         1.01574 |         0.01723 |
|values_float    | div_large       |         1.01619 |         0.01723 |
|values_float    | sqrt_small      |         0.92263 |         0.01565 |
|values_float    | sqrt_large      |         0.92308 |         0.01566 |
|values_float    | sin_small       |         2.14485 |         0.03638 |
|values_float    | sin_large       |         4.86370 |         0.08249 |
|values_float    | exp_small       |         1.15969 |         0.01967 |
|values_float    | exp_large       |         1.52946 |         0.02594 |
|values_int      | add_small       |         0.47997 |         0.03438 |
|values_int      | add_large       |         0.99489 |         0.07126 |
|values_int      | mul_small       |         0.48125 |         0.03447 |
|values_int      | mul_large       |         0.99233 |         0.07108 |
|values_int      | div_small       |         0.57946 |         0.04150 |
|values_int      | div_large       |         1.10439 |         0.07910 |
 */
