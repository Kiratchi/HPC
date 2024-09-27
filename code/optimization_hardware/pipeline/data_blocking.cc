#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

auto rd = std::default_random_engine();
auto udist_int = std::uniform_int_distribution<long int> (0,100);
auto udist_float = std::uniform_real_distribution<double> (0.,100.);


template
  <size_t sz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    long int *a;
    double *b;

public:

    ArrayFixture<sz>()
    {
      this->a = new long int[sz];
      this->b = new double[sz];

      for ( size_t ix = 0; ix < sz; ++ix ) {
        a[ix] = udist_int(rd);
        b[ix] = udist_float(rd);
      }
    }

    ~ArrayFixture<sz>()
    {
      delete[] this->a;
      delete[] this->b;
    }
};


CELERO_MAIN


constexpr size_t sz = 200000;
typedef ArrayFixture<sz> Fixture;

// Integer

BASELINE_F(datadepint, empty, Fixture, nmbsmpl, nmbiter)
{
  long int s = 0;
  for ( int ix = 0; ix < sz; ++ix ) {
    celero::DoNotOptimizeAway(a[ix]);
  }
}

BENCHMARK_F(datadepint, var1unroll1, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  for (int ix = 0; ix < sz; ++ix)
    s0 += a[ix];
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepint, var1unroll2, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  for (int ix = 0; ix < sz; ix += 2) {
    s0 += a[ix+0];
    s0 += a[ix+1];
  }
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepint, var1unroll4, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  for (int ix = 0; ix < sz; ix += 4) {
    s0 += a[ix+0];
    s0 += a[ix+1];
    s0 += a[ix+2];
    s0 += a[ix+3];
  }
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepint, var1unroll8, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  for (int ix = 0; ix < sz; ix += 8) {
    s0 += a[ix+0];
    s0 += a[ix+1];
    s0 += a[ix+2];
    s0 += a[ix+3];
    s0 += a[ix+4];
    s0 += a[ix+5];
    s0 += a[ix+6];
    s0 += a[ix+7];
  }
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepint, var2unroll2, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  long int s1 = 0;
  for (int ix = 0; ix < sz; ix += 2) {
    s0 += a[ix+0];
    s1 += a[ix+1];
  }
  celero::DoNotOptimizeAway(s0 + s1);
}

BENCHMARK_F(datadepint, var2unroll4, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  long int s1 = 0;
  for (int ix = 0; ix < sz; ix += 4) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s0 += a[ix+2];
    s1 += a[ix+3];
  }
  celero::DoNotOptimizeAway(s0 + s1);
}

BENCHMARK_F(datadepint, var2unroll8, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  long int s1 = 0;
  for (int ix = 0; ix < sz; ix += 8) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s0 += a[ix+2];
    s1 += a[ix+3];
    s0 += a[ix+4];
    s1 += a[ix+5];
    s0 += a[ix+6];
    s1 += a[ix+7];
  }
  celero::DoNotOptimizeAway(s0 + s1);
}

BENCHMARK_F(datadepint, var4unroll4, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  long int s1 = 0;
  long int s2 = 0;
  long int s3 = 0;
  for (int ix = 0; ix < sz; ix += 4) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s2 += a[ix+2];
    s3 += a[ix+3];
  }
  celero::DoNotOptimizeAway(s0 + s1 + s2 + s3);
}

BENCHMARK_F(datadepint, var4unroll8, Fixture, nmbsmpl, nmbiter)
{
  long int s0 = 0;
  long int s1 = 0;
  long int s2 = 0;
  long int s3 = 0;
  for (int ix = 0; ix < sz; ix += 8) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s2 += a[ix+2];
    s3 += a[ix+3];
    s0 += a[ix+4];
    s1 += a[ix+5];
    s2 += a[ix+6];
    s3 += a[ix+7];
  }
  celero::DoNotOptimizeAway(s0 + s1 + s2 + s3);
}

// Floating point

BASELINE_F(datadepfloat, empty, Fixture, nmbsmpl, nmbiter)
{
  double s = 0;
  for ( int ix = 0; ix < sz; ++ix ) {
    celero::DoNotOptimizeAway(a[ix]);
  }
}

BENCHMARK_F(datadepfloat, var1unroll1, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  for (int ix = 0; ix < sz; ++ix)
    s0 += a[ix];
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepfloat, var1unroll2, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  for (int ix = 0; ix < sz; ix += 2) {
    s0 += a[ix+0];
    s0 += a[ix+1];
  }
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepfloat, var1unroll4, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  for (int ix = 0; ix < sz; ix += 4) {
    s0 += a[ix+0];
    s0 += a[ix+1];
    s0 += a[ix+2];
    s0 += a[ix+3];
  }
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepfloat, var1unroll8, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  for (int ix = 0; ix < sz; ix += 8) {
    s0 += a[ix+0];
    s0 += a[ix+1];
    s0 += a[ix+2];
    s0 += a[ix+3];
    s0 += a[ix+4];
    s0 += a[ix+5];
    s0 += a[ix+6];
    s0 += a[ix+7];
  }
  celero::DoNotOptimizeAway(s0);
}

BENCHMARK_F(datadepfloat, var2unroll2, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  double s1 = 0;
  for (int ix = 0; ix < sz; ix += 2) {
    s0 += a[ix+0];
    s1 += a[ix+1];
  }
  celero::DoNotOptimizeAway(s0 + s1);
}

BENCHMARK_F(datadepfloat, var2unroll4, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  double s1 = 0;
  for (int ix = 0; ix < sz; ix += 4) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s0 += a[ix+2];
    s1 += a[ix+3];
  }
  celero::DoNotOptimizeAway(s0 + s1);
}

BENCHMARK_F(datadepfloat, var2unroll8, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  double s1 = 0;
  for (int ix = 0; ix < sz; ix += 8) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s0 += a[ix+2];
    s1 += a[ix+3];
    s0 += a[ix+4];
    s1 += a[ix+5];
    s0 += a[ix+6];
    s1 += a[ix+7];
  }
  celero::DoNotOptimizeAway(s0 + s1);
}

BENCHMARK_F(datadepfloat, var4unroll4, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  double s1 = 0;
  double s2 = 0;
  double s3 = 0;
  for (int ix = 0; ix < sz; ix += 4) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s2 += a[ix+2];
    s3 += a[ix+3];
  }
  celero::DoNotOptimizeAway(s0 + s1 + s2 + s3);
}

BENCHMARK_F(datadepfloat, var4unroll8, Fixture, nmbsmpl, nmbiter)
{
  double s0 = 0;
  double s1 = 0;
  double s2 = 0;
  double s3 = 0;
  for (int ix = 0; ix < sz; ix += 8) {
    s0 += a[ix+0];
    s1 += a[ix+1];
    s2 += a[ix+2];
    s3 += a[ix+3];
    s0 += a[ix+4];
    s1 += a[ix+5];
    s2 += a[ix+6];
    s3 += a[ix+7];
  }
  celero::DoNotOptimizeAway(s0 + s1 + s2 + s3);
}

/* One example run yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|datadepint      | empty           |         1.00000 |       251.48509 |
|datadepfloat    | empty           |         1.00000 |       210.33710 |
|datadepint      | var1unroll1     |         0.28828 |        72.49865 |
|datadepint      | var1unroll2     |         0.25499 |        64.12658 |
|datadepint      | var1unroll4     |         0.25387 |        63.84524 |
|datadepint      | var1unroll8     |         0.25612 |        64.41096 |
|datadepint      | var2unroll2     |         0.20365 |        51.21483 |
|datadepint      | var2unroll4     |         0.19060 |        47.93333 |
|datadepint      | var2unroll8     |         0.21488 |        54.03992 |
|datadepint      | var4unroll4     |         0.19065 |        47.94512 |
|datadepint      | var4unroll8     |         0.21482 |        54.02280 |
|datadepfloat    | var1unroll1     |         0.95496 |       200.86420 |
|datadepfloat    | var1unroll2     |         0.95496 |       200.86420 |
|datadepfloat    | var1unroll4     |         0.95496 |       200.86250 |
|datadepfloat    | var1unroll8     |         0.95483 |       200.83544 |
|datadepfloat    | var2unroll2     |         0.47768 |       100.47368 |
|datadepfloat    | var2unroll4     |         0.47774 |       100.48615 |
|datadepfloat    | var2unroll8     |         0.47787 |       100.51452 |
|datadepfloat    | var4unroll4     |         0.28579 |        60.11314 |
|datadepfloat    | var4unroll8     |         0.28872 |        60.72840 |
 */
