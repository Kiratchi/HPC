#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{   
    //We begin by creatint the array
    int size = 10;

    int *asentries = (int*) malloc(sizeof(int) * size * size);
    int **as = (int**) malloc(sizeof(int*) * size);

    //Allocates the pointers correctly
    for ( size_t ix = 0, jx = 0; ix < size; ++ix, jx += size){
        as[ix] = asentries + jx;
    }

    //Sets the values in the array
    for ( int ix = 0; ix < size; ++ix){
        for ( int jx = 0; jx < size; ++jx){
            as[ix][jx] = ix * jx;
        }
    }

    // Quick test, should print 5*7=35
    printf("%i\n", as[5][7]);
    printf("%i\n", asentries[11]);

    //Here we just create a new file
    FILE *file = fopen("written_file.dat", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fwrite(asentries, sizeof(int), size*size, file);
    fclose(file);

    //End by remove the pointers/memory allocation.
    free(asentries);
    free(as);
    return 0;
}
