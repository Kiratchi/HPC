#include <random>
#include "celero/Celero.h"

extern "C"
{
}

constexpr size_t nmbsmpl = 100;
constexpr size_t nmbiter = 0;

auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<float> (-10000.,10000.);


template
  <size_t sz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    double aa[sz][sz];
    double *p;
    double **pp;
    double (*pa)[sz];

public:

    ArrayFixture<sz>()
    {
      p = new double[sz*sz];
      pp = new double*[sz];
      for ( size_t ix = 0; ix < sz; ++ix )
        pp[ix] = p + ix*sz;
      this->pa = (double (*)[sz]) malloc(sz*sz*sizeof(double));
    }

    ~ArrayFixture<sz>()
    {
      delete[] pp;
      delete[] p;
      free(pa);
    }
};


CELERO_MAIN


constexpr size_t sz = 1000;
typedef ArrayFixture<sz> Fixture;


BASELINE_F(ptr_arr, array2d_direct, Fixture, nmbsmpl, nmbiter)
{
  double (*aa)[sz] = this->aa;
  for (int ix = 0; ix < sz; ++ix)
    for (int jx = 0; jx < sz; ++jx)
      celero::DoNotOptimizeAway(++aa[ix][jx]);
}

BENCHMARK_F(ptr_arr, array2d_local, Fixture, nmbsmpl, nmbiter)
{
  for (int ix = 0; ix < sz; ++ix)
    for (int jx = 0; jx < sz; ++jx)
      celero::DoNotOptimizeAway(++this->aa[ix][jx]);
}

BENCHMARK_F(ptr_arr, ptrptr, Fixture, nmbsmpl, nmbiter)
{
  auto pp = this->pp;
  for (int ix = 0; ix < sz; ++ix)
    for (int jx = 0; jx < sz; ++jx)
      celero::DoNotOptimizeAway(++pp[ix][jx]);
}

BENCHMARK_F(ptr_arr, ptrptr_local, Fixture, nmbsmpl, nmbiter)
{
  auto pp = this->pp;
  for (int ix = 0; ix < sz; ++ix) {
    auto p = pp[ix];
    for (int jx = 0; jx < sz; ++jx)
      celero::DoNotOptimizeAway(++p[jx]);
  }
}

BENCHMARK_F(ptr_arr, ptr_lin, Fixture, nmbsmpl, nmbiter)
{
  auto p = this->p;
  for (int ix = 0; ix < sz; ++ix)
    for (int jx = 0; jx < sz; ++jx)
      celero::DoNotOptimizeAway(++p[ix*sz+jx]);
}

BENCHMARK_F(ptr_arr, ptrarr, Fixture, nmbsmpl, nmbiter)
{
  double (*pa)[sz] = this->pa;
  for (int ix = 0; ix < sz; ++ix)
    for (int jx = 0; jx < sz; ++jx)
      celero::DoNotOptimizeAway(++pa[ix][jx]);
}

/* One example run, using native architecture, yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|ptr_arr         | array2d_direct  |         1.00000 |      1661.04000 |
|ptr_arr         | array2d_local   |         0.92288 |      1532.94767 |
|ptr_arr         | ptrptr          |         1.14674 |      1904.78344 |
|ptr_arr         | ptrptr_local    |         0.90767 |      1507.68362 |
|ptr_arr         | ptr_lin         |         0.93655 |      1555.65497 |
|ptr_arr         | ptrarr          |         0.91693 |      1523.06358 |


 * Without native architecture, one example run yields

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|ptr_arr         | array2d_direct  |         1.00000 |      1525.51899 |
|ptr_arr         | array2d_local   |         0.94466 |      1441.09942 |
|ptr_arr         | ptrptr          |         1.00591 |      1534.53012 |
|ptr_arr         | ptrptr_local    |         1.08975 |      1662.42775 |
|ptr_arr         | ptr_lin         |         0.96046 |      1465.20571 |
|ptr_arr         | ptrarr          |         1.00982 |      1540.49718 |

 */
