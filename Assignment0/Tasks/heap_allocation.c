#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{   
    /* This one works for even larger,
     * values than the stack_allocation.
     * Sweet!*/
    int size = 99999999;

    int *as = (int*) malloc(sizeof(int) * size);
    for ( size_t ix = 0; ix < size; ++ix )
        as[ix] = 0;

    printf("%d\n", as[0]);

    free(as);
}
