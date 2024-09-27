#include <stdlib.h>
#include <stdio.h>

unsigned short
d_calculator( short p1[3], short p2[3])
{
    return p1[2];
}


int main(int argc, char const *argv[])
{
    short p1[3] = {1,2,3};
    short p2[3] = {1,2,4};
    short p3[3];
    //p3 = d_calculator(p1,p2);
    printf("%i\n", d_calculator(p1,p2) );
    return 0;
}
