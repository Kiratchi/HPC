#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void row_sums(double * sums, double ** matrix, size_t nrs, size_t ncs);
void col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs);


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

    printf("30 * 10 should be %f\n", as[30][10]);

    double rsums[1000];
    double csums[1000];

    row_sums(rsums, as, 1000, 1000);
    col_sums(csums, (const double **) as, 1000, 1000);

    srand(time(NULL));
    int random_index = rand() % 1000;

    printf("Sum of random row %f\n", rsums[random_index]); 
    printf("Sum of random column %f\n", csums[random_index]);

    free(asentries);
    free(as); 

    return 0; 
}

// 16.6 ms O0 benchmark 5000
// 11.8 ms 02 benchmark 5000 
// 11.5 ms O2 native benchmark 5000



void
row_sums(double * sums, double ** matrix, size_t nrs, size_t ncs)
{
  for ( size_t ix = 0; ix < nrs; ++ix ) {
    double sum = 0.;
    for ( size_t jx = 0; jx < ncs; ++jx )
      sum += matrix[ix][jx];
    sums[ix] = sum;
  }
}

void
col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{
  for ( size_t jx = 0; jx < ncs; ++jx ) {
    double sum = 0.;
    for ( size_t ix = 0; ix < nrs; ++ix )
      sum += matrix[ix][jx];
    sums[jx] = sum;
  }
}

void
new_col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{
  for ( size_t jx = 0; jx < ncs; ++jx ) {
    double sum = 0.;
    for ( size_t ix = 0; ix < nrs; ++ix )
      sum += matrix[ix][jx];
    sums[jx] = sum;
  }
}

// Add a reimplementation of the slower summation in a more
// effective way, and benchmark as before. 
// The benchmarking results arising from this reimplement might be
// surprising at first sight and will be clarified in the next task.


