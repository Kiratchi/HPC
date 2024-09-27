// #pragma once

#ifndef INCLUDED_INCLUDE_H
#define INCLUDED_INCLUDE_H

#include <math.h>
#include <stdlib.h>

static inline
double
rand_double()
{
    return(double)rand()/RAND_MAX * 2. - 1.;
}

#endif
