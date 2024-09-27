#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void row_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs);
void row_sums_unrolled2(double * sums, const double ** matrix, size_t nrs, size_t ncs);
void row_sums_unrolled4(double * sums, const double ** matrix, size_t nrs, size_t ncs);
void row_sums_unrolled8(double * sums, const double ** matrix, size_t nrs, size_t ncs);
void row_sums_unrolled4_2(double * sums, const double ** matrix, size_t nrs, size_t ncs);



int main(int argc, char const *argv[])
{
    double *asentries = (double*) malloc(sizeof(double) * 1000 * 1000);
    double **as = (double**) malloc(sizeof(double*) * 1000);

    for (int ix = 0; ix < 1000; ++ix) {
        as[ix] = asentries + ix * 1000;
    }

    for (int ix = 0; ix < 1000; ++ix){
      for (int jx = 0; jx < 1000; ++jx){
        as[ix][jx] = ix*jx;
      }
    }

    double rsums[1000];

    srand(time(NULL));
    int random_index = rand() % 1000; 

    row_sums_unrolled8(rsums, (const double **) as, 1000, 1000);
    printf("Sum of random row %f\n", rsums[random_index]); 

    free(asentries);
    free(as); 

    return 0; 
}

// 11.8 ms O0 without function 2500 benchmark
// 14.2 ms O0 RS benchmark 2500
// 13.2 ms O0 2 benchmark 2500
// 13.1 ms O0 4 benchmark 2500
// 12.7 ms O0 8 benchmark 2500
// 13.1 ms O0 4.2 benchmank 2500

// 9.5 ms O2 without function 2500 native benchmark
// 10.4 ms O2 2 native benchmark 2500
// 10.3 ms O2 4 native benchmark 2500
// 10.1 ms O2 8 native benchmark 2500
// 9.8 ms O2 4.2 native benchmark 2500


void
row_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum = 0.;
    for ( size_t jx = 0; jx < ncs; ++jx )
      sum += matrix[ix][jx];
    sums[ix] = sum;
  }
}

void
row_sums_unrolled2(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum0 = 0.;
    double sum1 = 0.;
    for ( size_t jx = 0; jx < ncs; jx += 2 ) {
      sum0 += matrix[ix][jx];
      sum1 += matrix[ix][jx+1];
    }
    sums[ix] = sum0 + sum1;
  }
}

void
row_sums_unrolled4(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum0 = 0.;
    double sum1 = 0.;
    double sum2 = 0.;
    double sum3 = 0.;
    for ( size_t jx = 0; jx < ncs; jx += 4 ) {
      sum0 += matrix[ix][jx];
      sum1 += matrix[ix][jx+1];
      sum2 += matrix[ix][jx+2];
      sum3 += matrix[ix][jx+3];
    }
    sums[ix] = sum0 + sum1 + sum2 + sum3;
  }
}

void
row_sums_unrolled8(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum0 = 0.;
    double sum1 = 0.;
    double sum2 = 0.;
    double sum3 = 0.;
    double sum4 = 0.;
    double sum5 = 0.;
    double sum6 = 0.;
    double sum7 = 0.;
    for ( size_t jx = 0; jx < ncs; jx += 8 ) {
      sum0 += matrix[ix][jx];
      sum1 += matrix[ix][jx+1];
      sum2 += matrix[ix][jx+2];
      sum3 += matrix[ix][jx+3];
      sum4 += matrix[ix][jx+4];
      sum5 += matrix[ix][jx+5];
      sum6 += matrix[ix][jx+6];
      sum7 += matrix[ix][jx+7];
    }
    sums[ix] = sum0 + sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7;
  }
}


void
row_sums_unrolled4_2(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum[4] = {0.,0.,0.,0.};
    for ( size_t jx = 0; jx < ncs; jx += 4 ) {
      sum[0] += matrix[ix][jx];
      sum[1] += matrix[ix][jx+1];
      sum[2] += matrix[ix][jx+2];
      sum[3] += matrix[ix][jx+3];
    }
    sums[ix] = sum[0] + sum[1] + sum[2] + sum[3];
  }
} 