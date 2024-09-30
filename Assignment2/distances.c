#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
// #include <time.h>

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
    
    const int block_size = 10; 
    const int char_per_row = 24;
    
    char *cells = (char*) malloc(sizeof(char) * char_per_row * block_size);
    int *result = (int*) malloc(sizeof(int) * 3465);    //Varför 3465? För att max distance är 34.64 och vi vill ha alla möjliga distanser i en lista
    float **rows = (float**) malloc(sizeof(float*) * block_size);
    for (int ix = 0; ix < block_size; ++ix){
        rows[ix] = (float*) malloc(sizeof(float) * 3);
    }


    FILE *file = fopen("cells", "r");

    if (file == NULL) {
        printf("Error opening file cells!\n");
        return 1;
    }
    
    fread(cells, sizeof(char), char_per_row * block_size, file);
    fclose(file);
    
   
    for (int ix = 0; ix < block_size; ++ix){
        char temp[8];

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
    for (int ix = 0; ix < block_size; ++ix){
        for (int jx = ix + 1; jx < block_size; ++jx){
            float dist = distance(rows[ix], rows[jx]);
            int dist_int = (int) (dist * 100);
            result[dist_int] += 1;
        }
    }

    // RESULT
    for (int ix = 0; ix < 3465; ++ix){
        if (result[ix] != 0){
            printf("%02d.%02d %d\n", ix/100, ix%100, result[ix]);
        }
    }

    free(result);
    free(cells);
    free(rows);

    return 0;
}

float distance(float c_1[3], float c_2[3]){

    float dist_1 = (c_1[0]-c_2[0]);
    float dist_2 = (c_1[1]-c_2[1]);
    float dist_3 = (c_1[2]-c_2[2]);

    float dist = sqrt(dist_1 * dist_1 + dist_2 * dist_2 + dist_3 * dist_3);
    
    return dist;
}