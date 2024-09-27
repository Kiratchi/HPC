#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

extern "C" {
double
dotproduct(
    const double *a,
    const double *b,
    const int sz,
    int r
    );

double
dotproduct_noopenmp(
    const double *a,
    const double *b,
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

//      for ( size_t ix = 0; ix < sz; ++ix ) {
//        a[ix] = udist(rd);
//        b[ix] = udist(rd);
//      }
    }

    ~ArrayFixture<sz>()
    {
      delete[] a;
      delete[] b;
    }
};



CELERO_MAIN


constexpr size_t sz = 100000000;
typedef ArrayFixture<sz> Fixture;


BASELINE_F(speedup, sequential, Fixture, nmbsmpl, nmbiter)
{
  celero::DoNotOptimizeAway(dotproduct_noopenmp(
        const_cast<const double *> (a),
        const_cast<const double *> (b),
        sz
        ));
}

#define PAR_BENCH(R)                                               \
  BENCHMARK_F(speedup, parallel_ ## R, Fixture, nmbsmpl, nmbiter) \
  {                                                                \
    celero::DoNotOptimizeAway(dotproduct(                          \
          const_cast<const double *> (a),                          \
          const_cast<const double *> (b),                          \
          sz, R                                                    \
          ));                                                      \
  }

PAR_BENCH(1)
PAR_BENCH(2)
PAR_BENCH(3)
PAR_BENCH(4)
PAR_BENCH(5)
PAR_BENCH(6)
PAR_BENCH(7)
PAR_BENCH(8)
PAR_BENCH(9)
PAR_BENCH(10)
PAR_BENCH(11)
PAR_BENCH(12)
PAR_BENCH(13)
PAR_BENCH(14)
PAR_BENCH(15)
PAR_BENCH(16)
PAR_BENCH(17)
PAR_BENCH(18)
PAR_BENCH(19)
PAR_BENCH(20)
PAR_BENCH(21)
PAR_BENCH(22)
PAR_BENCH(23)
PAR_BENCH(24)
PAR_BENCH(25)
PAR_BENCH(26)
PAR_BENCH(27)
PAR_BENCH(28)
PAR_BENCH(29)
PAR_BENCH(30)
PAR_BENCH(40)
PAR_BENCH(50)
PAR_BENCH(60)


/* One example run yields:

|   Experiment    |    Baseline     |  us/Iteration   |
|:---------------:|:---------------:|:---------------:|
| sequential      |         1.00000 |    247239.50000 |
| parallel_1      |         1.02856 |    254300.50000 |
| parallel_2      |         0.38244 |     94553.50000 |
| parallel_3      |         0.22571 |     55804.16667 |
| parallel_4      |         0.17156 |     42415.57143 |
| parallel_5      |         0.13583 |     33583.00000 |
| parallel_6      |         0.11194 |     27675.77778 |
| parallel_7      |         0.09659 |     23881.50000 |
| parallel_8      |         0.08799 |     21754.70000 |
| parallel_9      |         0.07450 |     18419.70000 |
| parallel_10     |         0.07593 |     18773.70000 |
| parallel_11     |         0.06923 |     17116.72727 |
| parallel_12     |         0.06558 |     16214.18182 |
| parallel_13     |         0.06260 |     15478.36364 |
| parallel_14     |         0.05966 |     14750.27273 |
| parallel_15     |         0.05521 |     13649.16667 |
| parallel_16     |         0.05302 |     13108.83333 |
| parallel_17     |         0.05150 |     12733.50000 |
| parallel_18     |         0.04963 |     12271.25000 |
| parallel_19     |         0.04824 |     11927.00000 |
| parallel_20     |         0.04702 |     11624.16667 |
| parallel_21     |         0.04598 |     11368.66667 |
| parallel_22     |         0.04515 |     11162.16667 |
| parallel_23     |         0.04420 |     10928.83333 |
| parallel_24     |         0.04353 |     10762.25000 |
| parallel_25     |         0.04263 |     10540.58333 |
| parallel_26     |         0.04186 |     10350.50000 |
| parallel_27     |         0.04144 |     10244.58333 |
| parallel_28     |         0.04082 |     10091.66667 |
| parallel_29     |         0.04049 |     10011.50000 |
| parallel_30     |         0.03997 |      9882.16667 |
| parallel_40     |         0.03713 |      9180.16667 |
| parallel_50     |         0.03638 |      8994.25000 |
| parallel_60     |         0.03442 |      8509.08333 |


# Fitting the result with Julia. We fit a + b*r + c*r^2 against r*t(r).

times = [1 1.02856; 2 0.38244; 3 0.22571; 4 0.17156; 5 0.13583; 6 0.11194; 7 0.09659; 8 0.08799; 9 0.07450; 10 0.07593; 11 0.06923; 12 0.06558; 13 0.06260; 14 0.05966; 15 0.05521; 16 0.05302; 17 0.05150; 18 0.04963; 19 0.04824; 20 0.04702; 21 0.04598; 22 0.04515; 23 0.04420; 24 0.04353; 25 0.04263; 26 0.04186; 27 0.04144; 28 0.04082; 29 0.04049; 30 0.03997; 40 0.03713; 50 0.03638; 60 0.03442];

# The data looks very much like a hyperbola, confirming the naive formula for runtime.
using Plots
plot(times[:,1], times[:,2])

using CurveFit
(tpar, tseq, tsync) = poly_fit(times[:,1], times[:,1] .* times[:,2], 2)

# tseq = 0.0073284487567555285
# tpar = 0.6846550459290683
# tsync = 0.00028536855138071017
# sqrt(tpar/tsync) = 48.981586908584426
# tseq + 2 * sqrt(tpar*tsync) = 0.03528405775761772

t(r) = tseq + tpar/r + tsync*r

# t(100) = 0.04271185435411723
# t(200) = 0.06782543426254291
# t(300) = 0.09522119765739881
# t(318) = 0.10022865138490648
# t(3478) = 1.000037123548552

 */
