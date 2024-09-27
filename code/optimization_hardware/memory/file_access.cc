#include <cassert>
#include <cstring>
#include <fstream>
#include <random>
#include "celero/Celero.h"


constexpr size_t nmbsmpl = 0;
constexpr size_t nmbiter = 0;


auto rd = std::default_random_engine();
auto udist = std::uniform_real_distribution<float> (-10000.,10000.);

template
  <size_t sz>
class ArrayFileFixture
    : public celero::TestFixture
{
public:
    float *source;
    float *zeros;
    float *destination;
    static std::fstream file;

public:

    ArrayFileFixture<sz>()
    {
      source      = new float[sz];
      zeros       = new float[sz];
      destination = new float[sz];

      for ( size_t ix = 0; ix < sz; ++ix ) {
        source[ix] = udist(rd);
        zeros[ix] = 0.;
      }

      file.rdbuf()->pubsetbuf(0,0);
      if ( !file.is_open() )
        file.open("./bench.dat",
            std::ios_base::trunc |
            std::ios_base::in | std::ios_base::out |
            std::ios_base::binary
            );
    }

    ~ArrayFileFixture<sz>()
    {
      delete[] source;
      delete[] zeros;
      delete[] destination;
    }

    virtual
    void
    setUp(
        const celero::TestFixture::ExperimentValue&
        )
    override
    {
      assert( file.is_open() );
      file.seekp(0, std::ios_base::beg);
      // NOTE: In C++ the function write should be avoided in favor of binary
      // mode streams. The use of reinterpret_cast is usually a hint that one
      // uses the wrong functionality.
      file.write(reinterpret_cast<const char*> (zeros), sz*sizeof(float));
      file.seekp(0, std::ios_base::beg);
    }
};

template <size_t sz>
std::fstream
ArrayFileFixture<sz>::
file = std::fstream();


CELERO_MAIN


constexpr size_t sz = 100000;
typedef ArrayFileFixture<sz> Fixture;


BASELINE_F(memfile, memory_assign, Fixture, nmbsmpl, nmbiter)
{
  auto end = source + sz;
  for ( auto a = source, b = destination; a != end; ++a, ++b )
    *b = *a;
  celero::DoNotOptimizeAway(destination);
}

BENCHMARK_F(memfile, iter_only, Fixture, nmbsmpl, nmbiter)
{
  auto end = source + sz;
  auto a = source;
  auto b = destination;
  for ( ; a != end; ++a, ++b );
  celero::DoNotOptimizeAway(a);
  celero::DoNotOptimizeAway(b);
}

BENCHMARK_F(memfile, memory_individual, Fixture, nmbsmpl, nmbiter)
{
  auto end = source + sz;
  for ( auto a = source, b = destination; a != end; ++a, ++b )
    std::memcpy(b, a, sizeof(float));
  celero::DoNotOptimizeAway(destination);
}

BENCHMARK_F(memfile, memory_block, Fixture, nmbsmpl, nmbiter)
{
  constexpr size_t bz = 1000;
  auto end = source + sz;
  static_assert ( sz % bz == 0 );
  for ( auto a = source, b = destination; a != end; a += bz, b += bz )
    std::memcpy(b, a, bz*sizeof(float));
  celero::DoNotOptimizeAway(destination);
}

BENCHMARK_F(memfile, memory_complete, Fixture, nmbsmpl, nmbiter)
{
  std::memcpy(destination, source, sz*sizeof(float));
  celero::DoNotOptimizeAway(destination);
}

BENCHMARK_F(memfile, file_individual, Fixture, nmbsmpl, nmbiter)
{
  auto end = source + sz;
  for ( auto a = source; a != end; ++a )
    file.write(reinterpret_cast<const char*> (a), sizeof(float));
}

BENCHMARK_F(memfile, file_block, Fixture, nmbsmpl, nmbiter)
{
  constexpr size_t bz = 1000;
  auto end = source + sz;
  static_assert ( sz % bz == 0 );
  for ( auto a = source; a != end; a += bz )
    file.write(reinterpret_cast<const char*> (a), bz*sizeof(float));
}

BENCHMARK_F(memfile, file_complete, Fixture, nmbsmpl, nmbiter)
{
  file.write(reinterpret_cast<const char*> (source), sz*sizeof(float));
}

BENCHMARK_F(memfile, file_ascii, Fixture, nmbsmpl, nmbiter)
{
  auto end = source + sz;
  for ( auto a = source; a != end; ++a )
    // In C this would be fprintf(file, "%f ", a).
    file << *a << " ";
}
/*

|   Experiment    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:---------------:|:---------------:|:---------------:|:---------------:|
| memory_assign   |         1.00000 |        15.29679 |        65373.19 |
| iter_only       |         0.00037 |         0.00568 |    176053727.33 |
| memory_individu |         0.99691 |        15.24950 |        65575.93 |
| memory_block    |         0.93139 |        14.24727 |        70188.87 |
| memory_complete |         0.79230 |        12.11963 |        82510.76 |
| file_individual |      9147.83342 |    139932.50000 |            7.15 |
| file_block      |        16.52617 |       252.79739 |         3955.74 |
| file_complete   |         5.44887 |        83.35027 |        11997.56 |
| file_ascii      |     24496.21552 |    374713.50000 |            2.67 |
 */
