#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

extern "C" {
double
sum(
    const double *a,
    const int sz
    );

double
sum_openmp(
    const double *a,
    const int sz
    );

double
sum_ij_nocollapse(
    const int sz
    );

double
sum_ij_collapse(
    const int sz
    );

void
matmul_outer(
    double *a,
    const double *const * m,
    const double *b,
    const int sz
    );

void
matmul_inner(
    double *a,
    const double *const * m,
    const double *b,
    const int sz
    );
}

auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<double> (-10000.,10000.);


template
  <size_t sz, size_t msz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    double *a;
    double *b;
    double *mentries;
    double **m;

public:

    ArrayFixture<sz, msz>()
    {
      a = new double[sz];
      b = new double[msz];
      mentries = new double[msz*msz];
      m = new double*[msz];

      for ( size_t ix = 0; ix < sz; ++ix )
        a[ix] = udist(rd);
      for ( size_t ix = 0; ix < msz; ++ix )
        b[ix] = udist(rd);
      for ( size_t ix = 0; ix < msz; ++ix ) {
        m[ix] = mentries + msz*ix; 
        for ( size_t jx = 0; jx < msz; ++jx )
          m[ix][jx] = udist(rd);
      }
    }

    ~ArrayFixture<sz, msz>()
    {
      delete[] a;
      delete[] b;
      delete[] m;
      delete[] mentries;
    }
};



CELERO_MAIN


constexpr size_t sz = 100000000;
constexpr size_t msz = 1000;
typedef ArrayFixture<sz, msz> Fixture;


BASELINE_F(sum, sequential, Fixture, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(sum(
        const_cast<const double *> (a),
        sz
        ));
}

BENCHMARK_F(sum, parallel, Fixture, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(sum_openmp(
        const_cast<const double *> (a),
        sz
        ));
}

BASELINE_F(sum_ij, nocollapse, Fixture, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(sum_ij_nocollapse(sz));
}

BENCHMARK_F(sum_ij, collapse, Fixture, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(sum_ij_collapse(sz));
}

BASELINE_F(matmul, outer, Fixture, nmbsmpl, nmbiter)
{
  matmul_outer(
      a,
      const_cast<const double *const *> (m),
      const_cast<const double *> (b),
      msz
      );
}

BENCHMARK_F(matmul, inner, Fixture, nmbsmpl, nmbiter)
{
  matmul_inner(
      a,
      const_cast<const double *const *> (m),
      const_cast<const double *> (b),
      msz
      );
}


/* One example run yields:
|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|sum             | sequential      |         1.00000 |     67326.46154 |
|sum             | parallel        |         0.25117 |     16910.30909 |
|sum_ij          | nocollapse      |         1.00000 |     71249.15385 |
|sum_ij          | collapse        |         0.51371 |     36601.55556 |
|matmul          | outer           |         1.00000 |        94.69054 |
|matmul          | inner           |        25.73428 |      2436.79268 |
*/
