#include <stdio.h>

int main(int argc, char const *argv[])
{   
    /*Here 999999 works, but not 9999999
     *Results in a segmentation fault.*/
    int size = 9999999;

    int as[size];
    for ( size_t ix = 0; ix < size; ++ix )
        as[ix] = 0;

    printf("%d\n", as[0]);

    return 0;
}