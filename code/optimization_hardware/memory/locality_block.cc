#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;

extern "C" {
double
dotproduct(
    const double ** a,
    const double ** b,
    const size_t sz,
    const size_t bz
    );
}

auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<double> (-10000.,10000.);


template
  <size_t sz>
class ContArrayFixture
    : public celero::TestFixture
{
public:
    double **a;
    double **b;
    double *entries_a;
    double *entries_b;

public:

    ContArrayFixture<sz>()
    {
      entries_a = new double[sz*sz];
      entries_b = new double[sz*sz];
      a         = new double*[sz];
      b         = new double*[sz];

      auto row_a = entries_a;
      auto row_b = entries_b;
      for ( size_t ix = 0; ix < sz; ++ix, row_a += sz, row_b += sz ) {
        a[ix] = row_a;
        b[ix] = row_b;
      }

      for ( size_t ix = 0; ix < sz; ++ix )
        for ( size_t jx = 0; jx < sz; ++jx ) {
          a[ix][jx] = udist(rd);
          b[ix][jx] = udist(rd);
        }
    }

    ~ContArrayFixture<sz>()
    {
      delete[] entries_a;
      delete[] entries_b;
      delete[] a;
      delete[] b;
    }
};

template
  <size_t sz>
class FragArrayFixture
    : public celero::TestFixture
{
public:
    double **a;
    double **b;

public:

    FragArrayFixture<sz>()
    {
      a = new double*[sz];
      b = new double*[sz];

      for ( size_t ix = 0; ix < sz; ++ix ) {
        a[ix] = new double[sz];
        b[ix] = new double[sz];
      }

      for ( size_t ix = 0; ix < sz; ++ix )
        for ( size_t jx = 0; jx < sz; ++jx ) {
          a[ix][jx] = udist(rd);
          b[ix][jx] = udist(rd);
        }
    }

    ~FragArrayFixture<sz>()
    {
      for ( size_t ix = 0; ix < sz; ++ix ) {
        delete[] a[ix];
        delete[] b[ix];
      }
      delete[] a;
      delete[] b;
    }
};


CELERO_MAIN


constexpr size_t sz = 1000;
typedef ContArrayFixture<sz> ContFixture;
typedef FragArrayFixture<sz> FragFixture;


BASELINE_F(cont_array_locality, noblock, ContFixture, nmbsmpl, nmbiter)
{
  auto a = this->a;
  auto b = this->b;
  double sum = 0.;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      sum += a[ix][jx] + b[jx][ix];
  celero::DoNotOptimizeAway(sum);
}

BASELINE_F(frag_array_locality, noblock, FragFixture, nmbsmpl, nmbiter)
{
  auto a = this->a;
  auto b = this->b;
  double sum = 0.;
  for ( size_t ix = 0; ix < sz; ++ix )
    for ( size_t jx = 0; jx < sz; ++jx )
      sum += a[ix][jx] + b[jx][ix];
  celero::DoNotOptimizeAway(sum);
}

#define BZ_BENCH(BZ)                                                            \
  BENCHMARK_F(cont_array_locality, block_ ## BZ, ContFixture, nmbsmpl, nmbiter) \
  {                                                                             \
    celero::DoNotOptimizeAway(dotproduct(                                       \
          const_cast<const double **> (a),                                      \
          const_cast<const double **> (b),                                      \
          sz, BZ                                                                \
          ));                                                                   \
  }                                                                             \
                                                                                \
  BENCHMARK_F(frag_array_locality, block_ ## BZ, FragFixture, nmbsmpl, nmbiter) \
  {                                                                             \
    celero::DoNotOptimizeAway(dotproduct(                                       \
          const_cast<const double **> (a),                                      \
          const_cast<const double **> (b),                                      \
          sz, BZ                                                                \
          ));                                                                   \
  }

BZ_BENCH(1)
BZ_BENCH(2)
BZ_BENCH(3)
BZ_BENCH(4)
BZ_BENCH(5)
BZ_BENCH(6)
BZ_BENCH(7)
BZ_BENCH(8)
BZ_BENCH(9)
BZ_BENCH(10)
BZ_BENCH(20)
BZ_BENCH(30)
BZ_BENCH(40)
BZ_BENCH(50)
BZ_BENCH(60)
BZ_BENCH(70)
BZ_BENCH(80)
BZ_BENCH(90)
BZ_BENCH(100)
BZ_BENCH(150)
BZ_BENCH(200)
BZ_BENCH(250)
BZ_BENCH(300)
BZ_BENCH(350)
BZ_BENCH(400)
BZ_BENCH(450)


/* One example run on a server yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|cont_array_loca | noblock         |         1.00000 |      1720.98270 |
|frag_array_loca | noblock         |         1.00000 |      1288.24675 |
|cont_array_loca | block_1         |         1.44416 |      2485.36769 |
|cont_array_loca | block_2         |         0.86493 |      1488.53074 |
|cont_array_loca | block_3         |         0.74445 |      1281.19114 |
|cont_array_loca | block_4         |         0.73405 |      1263.28322 |
|cont_array_loca | block_5         |         0.73048 |      1257.13550 |
|cont_array_loca | block_6         |         0.73797 |      1270.03857 |
|cont_array_loca | block_7         |         0.71696 |      1233.86792 |
|cont_array_loca | block_8         |         0.70116 |      1206.69152 |
|cont_array_loca | block_9         |         0.70343 |      1210.59788 |
|cont_array_loca | block_10        |         0.70405 |      1211.65707 |
|cont_array_loca | block_20        |         0.74635 |      1284.46341 |
|cont_array_loca | block_30        |         0.74734 |      1286.15084 |
|cont_array_loca | block_40        |         0.75955 |      1307.18006 |
|cont_array_loca | block_50        |         0.74760 |      1286.59840 |
|cont_array_loca | block_60        |         0.75116 |      1292.73370 |
|cont_array_loca | block_70        |         0.75761 |      1303.84196 |
|cont_array_loca | block_80        |         0.76575 |      1317.83862 |
|cont_array_loca | block_90        |         0.76363 |      1314.18548 |
|cont_array_loca | block_100       |         0.76472 |      1316.06868 |
|cont_array_loca | block_150       |         0.76340 |      1313.79893 |
|cont_array_loca | block_200       |         0.76538 |      1317.19947 |
|cont_array_loca | block_250       |         0.77211 |      1328.79508 |
|cont_array_loca | block_300       |         0.77556 |      1334.72253 |
|cont_array_loca | block_350       |         0.77500 |      1333.75890 |
|cont_array_loca | block_400       |         0.77672 |      1336.71585 |
|cont_array_loca | block_450       |         0.77674 |      1336.75706 |
|frag_array_loca | block_1         |         1.69120 |      2178.68312 |
|frag_array_loca | block_2         |         1.14079 |      1469.62469 |
|frag_array_loca | block_3         |         1.01445 |      1306.86337 |
|frag_array_loca | block_4         |         1.02346 |      1318.46939 |
|frag_array_loca | block_5         |         0.98709 |      1271.61625 |
|frag_array_loca | block_6         |         0.96751 |      1246.39607 |
|frag_array_loca | block_7         |         0.95739 |      1233.35537 |
|frag_array_loca | block_8         |         0.98911 |      1274.21229 |
|frag_array_loca | block_9         |         0.94852 |      1221.92371 |
|frag_array_loca | block_10        |         0.95344 |      1228.26902 |
|frag_array_loca | block_20        |         0.95248 |      1227.02368 |
|frag_array_loca | block_30        |         0.95206 |      1226.48813 |
|frag_array_loca | block_40        |         0.96389 |      1241.72386 |
|frag_array_loca | block_50        |         0.95259 |      1227.16711 |
|frag_array_loca | block_60        |         0.96169 |      1238.89189 |
|frag_array_loca | block_70        |         0.96477 |      1242.85979 |
|frag_array_loca | block_80        |         0.97176 |      1251.86133 |
|frag_array_loca | block_90        |         0.97555 |      1256.75272 |
|frag_array_loca | block_100       |         0.97423 |      1255.05067 |
|frag_array_loca | block_150       |         0.96772 |      1246.65775 |
|frag_array_loca | block_200       |         0.96347 |      1241.18801 |
|frag_array_loca | block_250       |         0.97093 |      1250.79564 |
|frag_array_loca | block_300       |         0.97791 |      1259.78989 |
|frag_array_loca | block_350       |         0.97566 |      1256.88919 |
|frag_array_loca | block_400       |         0.97954 |      1261.88827 |
|frag_array_loca | block_450       |         0.98812 |      1272.94101 |
  
   One example run on a laptop yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|cont_array_loca | noblock         |         1.00000 |      2275.29878 |
|frag_array_loca | noblock         |         1.00000 |      2181.25153 |
|cont_array_loca | block_1         |         1.63747 |      3725.73438 |
|cont_array_loca | block_2         |         1.07674 |      2449.90323 |
|cont_array_loca | block_3         |         0.89273 |      2031.22358 |
|cont_array_loca | block_4         |         0.76763 |      1746.59609 |
|cont_array_loca | block_5         |         0.80074 |      1821.91182 |
|cont_array_loca | block_6         |         0.78465 |      1785.31058 |
|cont_array_loca | block_7         |         0.82901 |      1886.24916 |
|cont_array_loca | block_8         |         0.82411 |      1875.09441 |
|cont_array_loca | block_9         |         0.84344 |      1919.06918 |
|cont_array_loca | block_10        |         0.85445 |      1944.13588 |
|cont_array_loca | block_20        |         0.86685 |      1972.34711 |
|cont_array_loca | block_30        |         0.99498 |      2263.86543 |
|cont_array_loca | block_40        |         1.05359 |      2397.22997 |
|cont_array_loca | block_50        |         1.03926 |      2364.63457 |
|cont_array_loca | block_60        |         1.01291 |      2304.67718 |
|cont_array_loca | block_70        |         1.08933 |      2478.55231 |
|cont_array_loca | block_80        |         1.15026 |      2617.19000 |
|cont_array_loca | block_90        |         1.16207 |      2644.06683 |
|cont_array_loca | block_100       |         1.19007 |      2707.76092 |
|cont_array_loca | block_150       |         1.18669 |      2700.07798 |
|cont_array_loca | block_200       |         1.18832 |      2703.77574 |
|cont_array_loca | block_250       |         1.18076 |      2686.58391 |
|cont_array_loca | block_300       |         1.15231 |      2621.85920 |
|cont_array_loca | block_350       |         1.07649 |      2449.32759 |
|cont_array_loca | block_400       |         1.05242 |      2394.57919 |
|cont_array_loca | block_450       |         0.99665 |      2267.68243 |
|frag_array_loca | block_1         |         1.85617 |      4048.76632 |
|frag_array_loca | block_2         |         1.30065 |      2837.05198 |
|frag_array_loca | block_3         |         0.90720 |      1978.82796 |
|frag_array_loca | block_4         |         0.85739 |      1870.17320 |
|frag_array_loca | block_5         |         0.90041 |      1964.01471 |
|frag_array_loca | block_6         |         0.86833 |      1894.04778 |
|frag_array_loca | block_7         |         0.93975 |      2049.82158 |
|frag_array_loca | block_8         |         0.92499 |      2017.64576 |
|frag_array_loca | block_9         |         0.93408 |      2037.47423 |
|frag_array_loca | block_10        |         0.92630 |      2020.49897 |
|frag_array_loca | block_20        |         0.92085 |      2008.59630 |
|frag_array_loca | block_30        |         1.14865 |      2505.49199 |
|frag_array_loca | block_40        |         1.19957 |      2616.57393 |
|frag_array_loca | block_50        |         1.23211 |      2687.54617 |
|frag_array_loca | block_60        |         1.23617 |      2696.39461 |
|frag_array_loca | block_70        |         1.26690 |      2763.42686 |
|frag_array_loca | block_80        |         1.26301 |      2754.94836 |
|frag_array_loca | block_90        |         1.24751 |      2721.14118 |
|frag_array_loca | block_100       |         1.24333 |      2712.02036 |
|frag_array_loca | block_150       |         1.24080 |      2706.49545 |
|frag_array_loca | block_200       |         1.22200 |      2665.49412 |
|frag_array_loca | block_250       |         1.21402 |      2648.08764 |
|frag_array_loca | block_300       |         1.20290 |      2623.83596 |
|frag_array_loca | block_350       |         1.15659 |      2522.81758 |
|frag_array_loca | block_400       |         1.16410 |      2539.20530 |
|frag_array_loca | block_450       |         1.14813 |      2504.36923 |
 */
