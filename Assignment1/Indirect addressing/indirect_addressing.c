#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char const *argv[])
{

    size_t size = 1000000;
    
    double *y = (double*) malloc(sizeof(double) * size);
    double *x = (double*) malloc(sizeof(double) * size);
    // double p[size];
    int a = 2;

    // // One function for this one
    // for ( size_t ix = 0; ix < size; ++ix  ) {
    //     p[ix] = ix;
    // }

    // Next for this one, right?
    // size_t size_jump = 1000;
    // for ( size_t jx = 0, kx = 0; jx < size_jump; ++jx)
    //     for ( size_t ix = jx; ix < size; ix += size_jump, ++kx)
    //         p[ix] = kx;

    // indirect
    // for ( size_t kx = 0; kx < size; ++kx ) {
    //     size_t jx = p[kx];
    //     y[jx] += a * x[jx];
    // }

    // direct
    for ( size_t kx = 0; kx < size; ++kx ) {
        y[kx] += a * x[kx];
    }

    // This is for the benchmarking, yaooo!
    srand(time(NULL));
    int random_index = rand() % size; 
    printf("Random element %f\n", y[random_index]); 

    free(y);
    free(x);

    return 0;
}

// Indirect addressing
// 25.3 ms O0
// 20.7 ms O2
// 20.3 ms O2 native

// 31.6 ms O0
// 28.9 ms O2
// 29.7 ms O2 native

// Direct addressing.
// 17.5 ms O0
// 15.6 ms O2
// 15.4 ms O2 native

