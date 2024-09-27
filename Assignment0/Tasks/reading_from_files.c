#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{   
    // Begin by opening and reading the file, notice .dat.
    FILE *file =  fopen("written_file.dat", "r");
    if ( file == NULL ) {
        printf("error opening file\n");
        return -1;
    }

    // Here we format how the integer that we are fetching are
    // Going to look like. Exactly as we pushed it.
    int size = 10;
    int *asentries = (int*) malloc( sizeof(int) * size * size );

    // Read it.
    fread((int*) asentries, sizeof(int), size*size, file);
    fclose(file);

    // Here we just create the pointer to each row, as earlier
    int **as = (int**) malloc( sizeof(int*) * size);
    for ( size_t ix = 0, jx = 0; ix < size; ++ix, jx += size )
        as[ix] = asentries + jx;

    // Here we print the amazing file that we fetched!
    for ( int ix = 0; ix < size; ++ix){
        for ( int jx = 0; jx < size; ++jx){
            printf("%x ", as[ix][jx]);
        } printf("\n");
    }

    // Never forget to free the memory-allocation
    free(asentries);
    free(as);

    return 0;
}
