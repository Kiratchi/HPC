#include <stdint.h>
#include <stdio.h>

// From https://rosettacode.org/wiki/Isqrt_(integer_square_root)_of_X#C

/* Upper ranges for the variables
r in [0,x]
z in [0,x]
t in [-x,x]
*/
unsigned short
isqrt(unsigned int x) {
    unsigned int q = 1, r = 0;
    while (q <= x) {
        q <<= 2;
    }
    while (q > 1) {
        int t;      //Tillräckigt stort???
        q >>= 2;
        t = x - r - q;
        r >>= 1;
        if (t >= 0) {
            x = t;
            r += q;
        }
    }
    return r;
}

/*
int main() {
    short p;
    short n;

    printf("Integer square root for numbers 0 to 65:\n");
    for (n = 0; n <= 65; n++) {
        printf("%lld ", isqrt(n));
    }
    printf("\n");
}
*/