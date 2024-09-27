#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mul_cpx(double *a_re, double *a_im, double *b_re, double *b_im, double *c_re, double *c_im);

int main(int argc, char const *argv[])
{
    
    double as_re[30000];
    double as_im[30000];
    double bs_re[30000];
    double bs_im[30000];
    double cs_re[30000];
    double cs_im[30000];

    srand(time(NULL));
    int random_index = rand() % 30000;

    for (int ix = 0; ix < 30000; ++ix){
        bs_re[ix] = (double) rand() / RAND_MAX;
        bs_im[ix] = (double) rand() / RAND_MAX;
        cs_re[ix] = (double) rand() / RAND_MAX;
        cs_im[ix] = (double) rand() / RAND_MAX;

        mul_cpx(&as_re[ix], &as_im[ix], &bs_re[ix], &bs_im[ix], &cs_re[ix], &cs_im[ix]);
    }

    printf("Random entry of as: Re %f, Im %f\n", as_re[random_index], as_im[random_index]);


    return 0;
}
