#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

    //ASCII
    // + has number 00101011 in bytes, 053 in decimal, 2B in hexadecimal
    // -, 00101101, 055, 2D
    // ., 00101110, 056, 2E
    // " ", 00100000, 040, 20    (This is blankspace)
    // 0, 00110000, 060, 30
    // ...
    // 9, 00111001, 071, 39

    FILE *file = fopen("cells", "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    int size = 10;
    char *cells = (char*) malloc(sizeof(char) * 24*size);
    int *result = (int*) malloc(sizeof(int) * 3465);
    float *rows = (float*) malloc(sizeof(float) * 3 * size);
    float **vectors = (float**) malloc(sizeof(float*) * size);
    
    fread(cells, sizeof(char), 24*size, file);
    fclose(file);

    for (int ix = 0; ix < 24*size; ++ix){
        for (int jx = 0; jx < 3; ++jx){
            if (jx = 0){
                vectors[ix][0] = atof(cells[0+24*ix:7+24*ix]);
            }
            else if (jx = 1){
                vectors[ix][1] = atof(cells[8+24*ix:15+24*ix]);
            }
            else {
                vectors[ix][2] = atof(cells[16+24*ix:23+24*ix]);
            }
        }
    }

    // for (int ix = 0; ix < 24*size; ++ix){
    //     printf("%c", cells[ix]);
    // }
    // printf("\n");
    

    // Howdy howdy½

    // hej hej



    // RESULT
    for (int ix = 0; ix < 34; ++ix){
        printf("%02d.%02d %d\n", ix/100, ix%100, result[ix]);
    }

    free(result);
    free(cells);
    free(vectors);
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