#include <x86intrin.h>
#include <random>
#include "celero/Celero.h"

constexpr size_t nmbsmpl = 1000;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();

auto udist =
  std::uniform_int_distribution<int64_t>(
    std::numeric_limits<int64_t>::min(),
    std::numeric_limits<int64_t>::max());

class VectorFixture
    : public celero::TestFixture
{
public:
    int64_t a[16];
    int64_t b[16];
    int64_t c[16];

public:

    VectorFixture()
    {
      for ( size_t ix = 0; ix < 16; ++ix ) {
        a[ix] = udist(rd);
        b[ix] = udist(rd);
      }
    }

    ~VectorFixture()
    {
    }
};

CELERO_MAIN


BASELINE_F(vec8, individual, VectorFixture, nmbsmpl, nmbiter)
{
   c[0] = a[0] + b[0];
   c[1] = a[1] + b[1];
   c[2] = a[2] + b[2];
   c[3] = a[3] + b[3];
   c[4] = a[4] + b[4];
   c[5] = a[5] + b[5];
   c[5] = a[5] + b[5];
   c[6] = a[6] + b[6];
   c[7] = a[7] + b[7];
}

BENCHMARK_F(vec8, forloop, VectorFixture, nmbsmpl, nmbiter)
{
  for ( int ix = 0; ix < 8; ++ix )
    c[ix] = a[ix] + b[ix];

  celero::DoNotOptimizeAway(c);
}

BENCHMARK_F(vec8, add2, VectorFixture, nmbsmpl, nmbiter)
{
  __m128i ar0 = _mm_loadu_si128((__m128i *)(a));
  __m128i br0 = _mm_loadu_si128((__m128i *)(b));

  _mm_storeu_si128((__m128i *)(c), _mm_add_epi64(ar0, br0));

  __m128i ar1 = _mm_loadu_si128((__m128i *)(a+2));
  __m128i br1 = _mm_loadu_si128((__m128i *)(b+2));

  _mm_storeu_si128((__m128i *)(c+2), _mm_add_epi64(ar1, br1));

  __m128i ar2 = _mm_loadu_si128((__m128i *)(a+4));
  __m128i br2 = _mm_loadu_si128((__m128i *)(b+4));

  _mm_storeu_si128((__m128i *)(c+4), _mm_add_epi64(ar2, br2));

  __m128i ar3 = _mm_loadu_si128((__m128i *)(a+6));
  __m128i br3 = _mm_loadu_si128((__m128i *)(b+6));

  _mm_storeu_si128((__m128i *)(c+6), _mm_add_epi64(ar3, br3));

  celero::DoNotOptimizeAway(c);
}

BENCHMARK_F(vec8, add4, VectorFixture, nmbsmpl, nmbiter)
{
  __m256i ar0 = _mm256_loadu_si256((__m256i const*)(a));
  __m256i br0 = _mm256_loadu_si256((__m256i const*)(b));

  _mm256_storeu_si256((__m256i *)(c), _mm256_add_epi64(ar0, br0));

  __m256i ar1 = _mm256_loadu_si256((__m256i const*)(a+4));
  __m256i br1 = _mm256_loadu_si256((__m256i const*)(b+4));

  _mm256_storeu_si256((__m256i *)(c+4), _mm256_add_epi64(ar1, br1));

  celero::DoNotOptimizeAway(c);
}

BENCHMARK_F(vec8, add8, VectorFixture, nmbsmpl, nmbiter)
{
  __m512i ar = _mm512_loadu_si512((__m512i *)(a));
  __m512i br = _mm512_loadu_si512((__m512i *)(b));

  _mm512_storeu_si512((__m512i *)(c), _mm512_add_epi64(ar, br));

  celero::DoNotOptimizeAway(c);
}


BASELINE_F(vec16, individual, VectorFixture, nmbsmpl, nmbiter)
{
   c[ 0] = a[ 0] + b[ 0];
   c[ 1] = a[ 1] + b[ 1];
   c[ 2] = a[ 2] + b[ 2];
   c[ 3] = a[ 3] + b[ 3];
   c[ 4] = a[ 4] + b[ 4];
   c[ 5] = a[ 5] + b[ 5];
   c[ 5] = a[ 5] + b[ 5];
   c[ 6] = a[ 6] + b[ 6];
   c[ 7] = a[ 7] + b[ 7];
   c[ 8] = a[ 8] + b[ 8];
   c[ 9] = a[ 9] + b[ 9];
   c[10] = a[10] + b[10];
   c[11] = a[11] + b[11];
   c[12] = a[12] + b[12];
   c[13] = a[13] + b[13];
   c[13] = a[13] + b[13];
   c[14] = a[14] + b[14];
   c[15] = a[15] + b[15];

  celero::DoNotOptimizeAway(c);
}

BENCHMARK_F(vec16, add4, VectorFixture, nmbsmpl, nmbiter)
{
  __m256i ar0 = _mm256_loadu_si256((__m256i const*)(a));
  __m256i br0 = _mm256_loadu_si256((__m256i const*)(b));

  _mm256_storeu_si256((__m256i *)(c), _mm256_add_epi64(ar0, br0));

  __m256i ar1 = _mm256_loadu_si256((__m256i const*)(a+4));
  __m256i br1 = _mm256_loadu_si256((__m256i const*)(b+4));

  _mm256_storeu_si256((__m256i *)(c+4), _mm256_add_epi64(ar1, br1));

  __m256i ar2 = _mm256_loadu_si256((__m256i const*)(a+8));
  __m256i br2 = _mm256_loadu_si256((__m256i const*)(b+8));

  _mm256_storeu_si256((__m256i *)(c+8), _mm256_add_epi64(ar2, br2));

  __m256i ar3 = _mm256_loadu_si256((__m256i const*)(a+12));
  __m256i br3 = _mm256_loadu_si256((__m256i const*)(b+12));

  _mm256_storeu_si256((__m256i *)(c+12), _mm256_add_epi64(ar3, br3));

  celero::DoNotOptimizeAway(c);
}

BENCHMARK_F(vec16, add8, VectorFixture, nmbsmpl, nmbiter)
{
  __m512i ar0 = _mm512_loadu_si512((__m512i *)(a));
  __m512i br0 = _mm512_loadu_si512((__m512i *)(b));

  _mm512_storeu_si512((__m512i *)(c), _mm512_add_epi64(ar0, br0));

  __m512i ar1 = _mm512_loadu_si512((__m512i *)(a+8));
  __m512i br1 = _mm512_loadu_si512((__m512i *)(b+8));

  _mm512_storeu_si512((__m512i *)(c+8), _mm512_add_epi64(ar1, br1));

  celero::DoNotOptimizeAway(c);
}

/* One example run yields:

|     Group      |   Experiment    |    Baseline     |  us/Iteration   |
|:--------------:|:---------------:|:---------------:|:---------------:|
|vec8            | individual      |         1.00000 |         0.00922 |
|vec16           | individual      |         1.00000 |         0.01208 |
|vec8            | forloop         |         1.12738 |         0.01040 |
|vec8            | add2            |         0.63854 |         0.00589 |
|vec8            | add4            |         0.61187 |         0.00564 |
|vec8            | add8            |         0.64330 |         0.00593 |
|vec16           | add4            |         0.50473 |         0.00610 |
|vec16           | add8            |         0.49448 |         0.00598 |

 */
