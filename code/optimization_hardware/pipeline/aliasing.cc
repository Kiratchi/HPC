#include <random>
#include "celero/Celero.h"

extern "C"
{
static constexpr double s_inc = 1e-8;

void
addmul_pointer(
    double *a, const double *b, const double *c,
    double *s, int sz
    );

void
addmul_local_copy(
    double *a, const double *b, const double *c,
    double *s, int sz
    );

// NOTE: We do not include the restrict keyword in the declaration. It is a
// semanitic guarantee that only needs to be stated in the function defintion.
// Moreover, the restrict keyword is not defined in C++, so it would otherwise
// be impossible to call C code that make restrict-related guarantees from
// C++.
void
addmul_restrict(
    double *a, const double *b, const double *c,
    double *s, int sz
    );
}

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<float> (-10000.,10000.);


template
  <size_t sz>
class ArrayFixture
    : public celero::TestFixture
{
public:
    double *a;
    double *b;
    double *c;

public:

    ArrayFixture<sz>()
    {
      this->a = new double[sz];
      this->b = new double[sz];
      this->c = new double[sz];

      for ( size_t ix = 0; ix < sz; ++ix ) {
        b[ix] = udist(rd);
        c[ix] = udist(rd);
      }
    }

    ~ArrayFixture<sz>()
    {
      delete[] this->a;
      delete[] this->b;
      delete[] this->c;
    }
};


CELERO_MAIN


constexpr size_t sz = 100000;
typedef ArrayFixture<sz> Fixture;


BASELINE_F(aliasing, inlined, Fixture, nmbsmpl, nmbiter)
{
  auto a = this->a;
  auto b = this->b;
  auto c = this->c;
  double svar = 0.;
  auto s = &svar;

  for (int ix = 0; ix < sz; ++ix) {
    a[ix] = b[ix] + *s * c[ix];
    *s += s_inc;
  }
  celero::DoNotOptimizeAway(a);
  celero::DoNotOptimizeAway(s);
}

BENCHMARK_F(aliasing, pointer, Fixture, nmbsmpl, nmbiter)
{
  // NOTE: a, b, c are defined in the class Fixture; this is legitimate syntax
  // in C++.
  double s = 0.;
  addmul_pointer(a, b, c, &s, sz);
}

BENCHMARK_F(aliasing, local_copy, Fixture, nmbsmpl, nmbiter)
{
  double s = 0.;
  addmul_local_copy(a, b, c, &s, sz);
}

BENCHMARK_F(aliasing, restricted, Fixture, nmbsmpl, nmbiter)
{
  double s = 0.;
  addmul_restrict(a, b, c, &s, sz);
}

BENCHMARK_F(aliasing, alised_pointer, Fixture, nmbsmpl, nmbiter)
{
  double s = 0.;
  addmul_pointer(a+1, b, a, &s, sz-1);
}

BENCHMARK_F(aliasing, alised_local_copy, Fixture, nmbsmpl, nmbiter)
{
  double s = 0.;
  addmul_local_copy(a+1, b, a, &s, sz-1);
}

BENCHMARK_F(aliasing, alised_s_pointer, Fixture, nmbsmpl, nmbiter)
{
  double *s = b + (sz>>1);
  addmul_pointer(a+1, b, a, s, sz-1);
}

BENCHMARK_F(aliasing, alised_s_local_copy, Fixture, nmbsmpl, nmbiter)
{
  double *s = b + (sz>>1);
  addmul_local_copy(a+1, b, a, s, sz-1);
}

/* One example run yields:
 *
|   Experiment    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:---------------:|:---------------:|:---------------:|:---------------:|
| inlined         |         1.00000 |       227.08981 |         4403.54 |
| pointer         |         1.00112 |       227.34317 |         4398.64 |
| local_copy      |         0.49250 |       111.84091 |         8941.27 |
| restricted      |         0.49267 |       111.87952 |         8938.19 |
| alised_pointer  |         1.43986 |       326.97849 |         3058.31 |
| alised_local_co |         1.43856 |       326.68120 |         3061.09 |
| alised_s_pointe |         1.43854 |       326.67801 |         3061.12 |
| alised_s_local_ |         1.43868 |       326.70942 |         3060.82 |
 */
