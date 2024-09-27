#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();

auto udist_float =
  std::uniform_real_distribution<float> (-10000.,10000.);
auto udist_double =
  std::uniform_real_distribution<double> (-10000.,10000.);
auto udist_char =
  std::uniform_int_distribution<signed char>(
    std::numeric_limits<signed char>::min(),
    std::numeric_limits<signed char>::max());
auto udist_short =
  std::uniform_int_distribution<short int>(
    std::numeric_limits<short int>::min(),
    std::numeric_limits<short int>::max());
auto udist_int =
  std::uniform_int_distribution<int>(
    std::numeric_limits<int>::min(),
    std::numeric_limits<int>::max());
auto udist_long =
  std::uniform_int_distribution<long int>(
    std::numeric_limits<long int>::min(),
    std::numeric_limits<long int>::max());

auto udist_float_pos =
  std::uniform_real_distribution<float> (0.,10000.);
auto udist_double_pos =
  std::uniform_real_distribution<double> (0.,10000.);
auto udist_char_pos =
  std::uniform_int_distribution<signed char>(
    0, std::numeric_limits<signed char>::max());
auto udist_short_pos =
  std::uniform_int_distribution<short int>(
    0, std::numeric_limits<short int>::max());
auto udist_int_pos =
  std::uniform_int_distribution<int>(
    0, std::numeric_limits<int>::max());
auto udist_long_pos =
  std::uniform_int_distribution<long int>(
    0, std::numeric_limits<long int>::max());

CELERO_MAIN

////////////////////////////////////////////////////////////
// add
////////////////////////////////////////////////////////////

BASELINE(types_add_float, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_float(rd));
  celero::DoNotOptimizeAway(udist_float(rd));
}

BENCHMARK(types_add_float, float, nmbsmpl, nmbiter)
{
  auto a = udist_float(rd);
  auto b = udist_float(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(types_add_float, double, nmbsmpl, nmbiter)
{
  auto a = udist_double(rd);
  auto b = udist_double(rd);
  celero::DoNotOptimizeAway(a+b);
}

BASELINE(types_add_int, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_int(rd));
  celero::DoNotOptimizeAway(udist_int(rd));
}

BENCHMARK(types_add_int, char, nmbsmpl, nmbiter)
{
  auto a = udist_char(rd);
  auto b = udist_char(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(types_add_int, short, nmbsmpl, nmbiter)
{
  auto a = udist_short(rd);
  auto b = udist_short(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(types_add_int, int, nmbsmpl, nmbiter)
{
  auto a = udist_int(rd);
  auto b = udist_int(rd);
  celero::DoNotOptimizeAway(a+b);
}

BENCHMARK(types_add_int, long, nmbsmpl, nmbiter)
{
  auto a = udist_long(rd);
  auto b = udist_long(rd);
  celero::DoNotOptimizeAway(a+b);
}

////////////////////////////////////////////////////////////
// mul
////////////////////////////////////////////////////////////

BASELINE(types_mul_float, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_float(rd));
  celero::DoNotOptimizeAway(udist_float(rd));
}

BENCHMARK(types_mul_float, float, nmbsmpl, nmbiter)
{
  auto a = udist_float(rd);
  auto b = udist_float(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(types_mul_float, double, nmbsmpl, nmbiter)
{
  auto a = udist_double(rd);
  auto b = udist_double(rd);
  celero::DoNotOptimizeAway(a*b);
}

BASELINE(types_mul_int, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_int(rd));
  celero::DoNotOptimizeAway(udist_int(rd));
}

BENCHMARK(types_mul_int, char, nmbsmpl, nmbiter)
{
  auto a = udist_char(rd);
  auto b = udist_char(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(types_mul_int, short, nmbsmpl, nmbiter)
{
  auto a = udist_short(rd);
  auto b = udist_short(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(types_mul_int, int, nmbsmpl, nmbiter)
{
  auto a = udist_int(rd);
  auto b = udist_int(rd);
  celero::DoNotOptimizeAway(a*b);
}

BENCHMARK(types_mul_int, long, nmbsmpl, nmbiter)
{
  auto a = udist_long(rd);
  auto b = udist_long(rd);
  celero::DoNotOptimizeAway(a*b);
}

////////////////////////////////////////////////////////////
// div
////////////////////////////////////////////////////////////

BASELINE(types_div_float, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_float(rd));
  celero::DoNotOptimizeAway(udist_float(rd));
}

BENCHMARK(types_div_float, float, nmbsmpl, nmbiter)
{
  auto a = udist_float(rd);
  auto b = udist_float(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(types_div_float, double, nmbsmpl, nmbiter)
{
  auto a = udist_double(rd);
  auto b = udist_double(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BASELINE(types_div_int, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_int(rd));
  celero::DoNotOptimizeAway(udist_int(rd));
}

BENCHMARK(types_div_int, char, nmbsmpl, nmbiter)
{
  auto a = udist_char(rd);
  auto b = udist_char(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(types_div_int, short, nmbsmpl, nmbiter)
{
  auto a = udist_short(rd);
  auto b = udist_short(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(types_div_int, int, nmbsmpl, nmbiter)
{
  auto a = udist_int(rd);
  auto b = udist_int(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

BENCHMARK(types_div_int, long, nmbsmpl, nmbiter)
{
  auto a = udist_long(rd);
  auto b = udist_long(rd);
  if ( b != 0 ) celero::DoNotOptimizeAway(a/b);
}

////////////////////////////////////////////////////////////
// sqrt
////////////////////////////////////////////////////////////

BASELINE(types_sqrt_float, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_float_pos(rd));
}

BENCHMARK(types_sqrt_float, float, nmbsmpl, nmbiter)
{
  auto a = udist_float_pos(rd);
  // OBSERVE: This is C++ and we use "overloading" to call sqrt(float).
  // In C you have to call sqrtf, since otherwise the argument will be
  // automatically converted to double.
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(types_sqrt_float, float_fake, nmbsmpl, nmbiter)
{
  auto a = udist_float_pos(rd);
  // This mimics the situation of calling sqrt(a) in C as opposed to C++.
  celero::DoNotOptimizeAway(sqrt(static_cast<double>(a)));
}

BENCHMARK(types_sqrt_float, double, nmbsmpl, nmbiter)
{
  auto a = udist_double_pos(rd);
  celero::DoNotOptimizeAway(sqrt(a));
}

BASELINE(types_sqrt_int, nullop, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(udist_int_pos(rd));
}

BENCHMARK(types_sqrt_int, char, nmbsmpl, nmbiter)
{
  auto a = udist_char_pos(rd);
  // OBSERVE: The result of these computations is a double. There is no isqrt
  // in C or C++ standard.
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(types_sqrt_int, char_fake, nmbsmpl, nmbiter)
{
  auto a = udist_char_pos(rd);
  celero::DoNotOptimizeAway(sqrt(static_cast<double>(a)));
}

BENCHMARK(types_sqrt_int, short, nmbsmpl, nmbiter)
{
  auto a = udist_short_pos(rd);
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(types_sqrt_int, short_fake, nmbsmpl, nmbiter)
{
  auto a = udist_short_pos(rd);
  celero::DoNotOptimizeAway(sqrt(static_cast<double>(a)));
}

BENCHMARK(types_sqrt_int, int, nmbsmpl, nmbiter)
{
  auto a = udist_int_pos(rd);
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(types_sqrt_int, int_fake, nmbsmpl, nmbiter)
{
  auto a = udist_int_pos(rd);
  celero::DoNotOptimizeAway(sqrt(static_cast<double>(a)));
}

BENCHMARK(types_sqrt_int, long, nmbsmpl, nmbiter)
{
  auto a = udist_long_pos(rd);
  celero::DoNotOptimizeAway(sqrt(a));
}

BENCHMARK(types_sqrt_int, long_fake, nmbsmpl, nmbiter)
{
  auto a = udist_long_pos(rd);
  celero::DoNotOptimizeAway(sqrt(static_cast<double>(a)));
}

/* One example run yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|types_add_float | nullop          |         1.00000 |         0.01139 |
|types_add_int   | nullop          |         1.00000 |         0.07867 |
|types_mul_float | nullop          |         1.00000 |         0.01053 |
|types_mul_int   | nullop          |         1.00000 |         0.07840 |
|types_div_float | nullop          |         1.00000 |         0.01053 |
|types_div_int   | nullop          |         1.00000 |         0.07877 |
|types_sqrt_floa | nullop          |         1.00000 |         0.00787 |
|types_sqrt_int  | nullop          |         1.00000 |         0.05569 |
|types_add_float | float           |         0.83992 |         0.00957 |
|types_add_float | double          |         1.45278 |         0.01655 |
|types_add_int   | char            |         0.44143 |         0.03473 |
|types_add_int   | short           |         0.45171 |         0.03554 |
|types_add_int   | int             |         0.99108 |         0.07797 |
|types_add_int   | long            |         0.93716 |         0.07373 |
|types_mul_float | float           |         0.90761 |         0.00956 |
|types_mul_float | double          |         1.57174 |         0.01655 |
|types_mul_int   | char            |         0.47995 |         0.03763 |
|types_mul_int   | short           |         0.46536 |         0.03648 |
|types_mul_int   | int             |         1.02297 |         0.08020 |
|types_mul_int   | long            |         0.95952 |         0.07523 |
|types_div_float | float           |         1.00036 |         0.01053 |
|types_div_float | double          |         1.64203 |         0.01729 |
|types_div_int   | char            |         0.46184 |         0.03638 |
|types_div_int   | short           |         0.47423 |         0.03735 |
|types_div_int   | int             |         1.00775 |         0.07938 |
|types_div_int   | long            |         1.02983 |         0.08112 |
|types_sqrt_floa | float           |         1.20708 |         0.00949 |
|types_sqrt_floa | float_fake      |         1.20757 |         0.00950 |
|types_sqrt_floa | double          |         1.41756 |         0.01115 |
|types_sqrt_int  | char            |         0.36712 |         0.02045 |
|types_sqrt_int  | char_fake       |         0.36685 |         0.02043 |
|types_sqrt_int  | short           |         0.37425 |         0.02084 |
|types_sqrt_int  | short_fake      |         0.37411 |         0.02084 |
|types_sqrt_int  | int             |         1.00548 |         0.05600 |
|types_sqrt_int  | int_fake        |         1.00712 |         0.05609 |
|types_sqrt_int  | long            |         0.83397 |         0.04645 |
|types_sqrt_int  | long_fake       |         0.83288 |         0.04639 |
 */
