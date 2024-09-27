#include "stddef.h"

double
dotproduct(
    const double *restrict *restrict a,
    const double *restrict *restrict b,
    const size_t sz,
    const size_t bz
    )
{
    double sum = 0.;
    for ( size_t ib = 0; ib < sz; ib += bz ) {
      size_t ie = ib + bz < sz ? ib + bz : sz;
      for ( size_t jb = 0; jb < sz; jb += bz ) {
        size_t je = jb + bz < sz ? jb + bz : sz;

        for ( size_t ix = ib; ix < ie; ++ix )
          for ( size_t jx = jb; jx < je; ++jx )
            sum += a[ix][jx] * b[jx][ix];
      }
    }
    return sum;
}
