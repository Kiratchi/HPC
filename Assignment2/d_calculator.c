#include <stdlib.h>
#include <stdio.h>
#include "isqrt.c"

unsigned short
d_calculator( short p1[3], short p2[3])
{
    unsigned short distance;
    unsigned short diffrence[3];
    unsigned int distance_square;    

    distance_square = 0;
    for(int i = 0; i < 3; ++i ) {   //Remove for loop senare och gör explicit
        diffrence[i] = p1[i]-p2[i];  //Kan vli negativt!!!
        distance_square += diffrence[i]^2/10;  //Kan avrunda fel
    }

    

    distance = isqrt(distance_square);
//    return distance;
    return distance;
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
