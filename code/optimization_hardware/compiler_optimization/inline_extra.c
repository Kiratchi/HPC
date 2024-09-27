#include <stdlib.h>

double
rand_double()
{
    return (double)rand()/RAND_MAX * 2. - 1.;
}
