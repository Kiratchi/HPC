#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char const *argv[])
{

    long int sum = 0;
    for (long int ix = 0; ix < 1000000000; ++ix){
        sum += ix;
    }

    printf("The sum is %li\n", sum);

    return 0;
}

// hyperfine
// -O0 - mean : 2.471 s - check again
// -O1 - mean : 69.5 ms
// -O2 - mean : 71.7 ms
// -O3 - mean : 70.6 ms
// -Os - mean : 280 ms (size) - check again
// -Og - mean : 279.6 ms (debug) - check again