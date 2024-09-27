#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mul_cpx(double *a_re, double *a_im, double *b_re, double *b_im, double *c_re, double *c_im) {
    *a_re = (*c_re) * (*b_re) - (*c_im) * (*b_im); 
    *a_im = (*c_re) * (*b_im) + (*c_im) * (*b_re); 
}