#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

extern "C" {
  void
  triangular_sum(
      double * a,
      const double * b,
      const int sz
      );

  void
  triangular_sum_static(
      double * a,
      const double * b,
      const int sz,
      const int chksz
      );

  void
  triangular_sum_dynamic(
      double * a,
      const double * b,
      const int sz,
      const int chksz
      );

  void
  triangular_sum_guided(
      double * a,
      const double * b,
      const int sz
      );
}

auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<double> (-10000.,10000.);


template
  <size_t sz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    double *a;
    double *b;

public:

    ArrayFixture<sz>()
    {
      a = new double[sz];
      b = new double[sz];

      for ( size_t ix = 0; ix < sz; ++ix )
        b[ix] = udist(rd);
    }

    ~ArrayFixture<sz>()
    {
      delete[] a;
      delete[] b;
    }
};



CELERO_MAIN


constexpr size_t sz = 10000;
typedef ArrayFixture<sz> Fixture;


BASELINE_F(triangular_sum, sequential, Fixture, nmbsmpl, nmbiter)
{
  triangular_sum(a, b, sz);
}

BENCHMARK_F(triangular_sum, openmp_sta10, Fixture, nmbsmpl, nmbiter)
{
  triangular_sum_static(a, b, sz, 10);
}

BENCHMARK_F(triangular_sum, openmp_sta1000, Fixture, nmbsmpl, nmbiter)
{
  triangular_sum_static(a, b, sz, 1000);
}

BENCHMARK_F(triangular_sum, openmp_dyn10, Fixture, nmbsmpl, nmbiter)
{
  triangular_sum_dynamic(a, b, sz, 10);
}

BENCHMARK_F(triangular_sum, openmp_dyn1000, Fixture, nmbsmpl, nmbiter)
{
  triangular_sum_dynamic(a, b, sz, 1000);
}

BENCHMARK_F(triangular_sum, openmp_guided, Fixture, nmbsmpl, nmbiter)
{
  triangular_sum_guided(a, b, sz);
}


/* One example run yields:
|   Experiment    |    Baseline     |  us/Iteration   |
|:---------------:|:---------------:|:---------------:|
| sequential      |         1.00000 |     33721.44828 |
| openmp_sta10    |         0.05282 |      1781.24503 |
| openmp_sta1000  |         0.21853 |      7369.06015 |
| openmp_dyn10    |         0.05071 |      1710.17808 |
| openmp_dyn1000  |         0.22871 |      7712.31061 |
| openmp_guided   |         0.09516 |      3208.82848 |
*/
