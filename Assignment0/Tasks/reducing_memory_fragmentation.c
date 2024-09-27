#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
// {
//     int size = 10;

//     int *asentries = (int*) malloc(sizeof(int) * size*size);
//     int **as = (int**) malloc(sizeof(int*) * size);
//     for ( size_t ix = 0, jx = 0; ix < size; ++ix, jx+=size )
//         as[ix] = asentries + jx;

//     for ( size_t ix = 0; ix < size; ++ix )
//         for ( size_t jx = 0; jx < size; ++jx )
//             as[ix][jx] = 0;

//     printf("What can I say\n");
//     printf("%d\n", as[0][0]);

//     for ( size_t ix = 0; ix < size; ++ix )
//         printf("%p\n", as[ix]);

//     free(as);
//     free(asentries);

//     return 0;
// }

{
    int size = 10;

    int ** as = (int**) malloc(sizeof(int*) * size);
    for ( size_t ix = 0; ix < size; ++ix )
        as[ix] = (int*) malloc(sizeof(int) * size);

    for ( size_t ix = 0; ix < size; ++ix )
        for ( size_t jx = 0; jx < size; ++jx )
            as[ix][jx] = 0;

    printf("%d\n", as[0][0]);

    for ( size_t ix = 0; ix < size; ++ix )
        printf("%p\n", as[ix]);

    for ( size_t ix = 0; ix < size; ++ix )
        free(as[ix]);
    free(as);
}
