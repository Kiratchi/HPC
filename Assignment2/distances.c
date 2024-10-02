#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// anvand float istallet for double, osb! eg sqrtf() istallet for sqrt() 
// kolla pa locality_block_auxillary.c

float distance(float c_1[3], float c_2[3]);

int main(int argc, char const *argv[])
{
    // Step 1, reading and parsing the file

    // We know that 256 bytes is usefull.
    // Should we use cachelines of some sort? Not shure?

    // Each row is 24 characters, with \n (new row) or terminate.

    /*  
      ---------ASCII---------
        byte        Dec     Hex 
    +   00101011    053     2B
    -   00101101    055     2D
    .   00101110    056     2E
    " " 00100000    040     20    (This is blankspace)
    0   00110000    060     30
    ...
    9    00111001    071     39
    */
    
    const int rows_per_block = 10000; 
    const int char_per_row = 24;
    const int max_distance = 3465; // = sqrt(20^2*3)
    
    char *cells = (char*) malloc(sizeof(char) * char_per_row * rows_per_block);
    int *result = (int*) malloc(sizeof(int) * max_distance);
    float **rows = (float**) malloc(sizeof(float*) * rows_per_block);
    for (int ix = 0; ix < rows_per_block; ++ix){
        rows[ix] = (float*) malloc(sizeof(float) * 3);
    }


    FILE *file = fopen("cells_1e4", "r");

    if (file == NULL) {
        printf("Error opening file cells!\n");
        return 1;
    }
    
    fread(cells, sizeof(char), char_per_row * rows_per_block, file);
    fclose(file);
    
   
    for (int ix = 0; ix < rows_per_block; ++ix){
        char temp[8];   //Borde vi annvända 3 olika temp så att operationerna inte behöver ske efter varandra?

        strncpy(temp, &cells[ix * char_per_row], 7);
        temp[7] = '\0';
        rows[ix][0] = atof(temp);

        strncpy(temp, &cells[ix * char_per_row + 8], 7);
        temp[7] = '\0';
        rows[ix][1] = atof(temp);

        strncpy(temp, &cells[ix * char_per_row + 16], 7);
        temp[7] = '\0';
        rows[ix][2] = atof(temp);
    }

    // // Print the rows
    // for (int ix = 0; ix < block_size; ++ix){
    //     printf("%f %f %f\n", rows[ix][0], rows[ix][1], rows[ix][2]);
    // }

    // Calculate the distances (inte kollat på vilka distanser som räknas...)
    for (int ix = 0; ix < rows_per_block; ++ix){
        for (int jx = ix + 1; jx < rows_per_block; ++jx){
            float dist = distance(rows[ix], rows[jx]) * 100;
            dist = round(dist);
            int dist_int = (int) (dist);
            result[dist_int] += 1;
        }
    }

    // Initialize the random number generator
    srand(time(NULL));
    // Generate a random index between 0 and rows_per_block
    int ix = rand() % (rows_per_block + 1);
    // Print the value at rows[ix][2]
    printf("Random value from rows[%i][2]: %f\n", ix, rows[ix][2]);

    // RESULT
    for (int ix = 0; ix < 3465; ++ix){
        if (result[ix] != 0){
            printf("%02d.%02d %d\n", ix/100, ix%100, result[ix]);
        }
    }

    // This seems to work/print the correct stuff
    // printf("%f\n", rows[6][2]);

    free(result);
    free(cells);
    free(rows);

    return 0;
}

float distance(float c_1[3], float c_2[3]){

    float dist_1 = (c_1[0]-c_2[0]);
    float dist_2 = (c_1[1]-c_2[1]);
    float dist_3 = (c_1[2]-c_2[2]);

    float dist = sqrtf(dist_1 * dist_1 + dist_2 * dist_2 + dist_3 * dist_3);
    
    return dist;
}