#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>

// anvand float istallet for double, osb! eg sqrtf() istallet for sqrt() 
// kolla pa locality_block_auxillary.c

float distance(float c_1[3], float c_2[3]);

int main(int argc, char const *argv[]) {

    // First for loop for calculating all the distances, and then append to some list
    
    const int rows_per_block = 5;
    int last_block_size;            // Lägg in det här senare!!! 
    int rows_per_file;
    const int char_per_row = 24;
    const int max_distance = 3465; // from sqrt(20^2*3)
    int *result = (int*) malloc(sizeof(int) * max_distance); // 4*3465

    FILE *file = fopen("cells", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file cells!\n");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    rows_per_file = ftell(file) / char_per_row;
    if (rows_per_file % rows_per_block != 0) {
        fprintf(stderr, "Number of rows not divisible by %i\n", rows_per_block);
        return 1;
    }

    char *cells = (char*) malloc(sizeof(char) * char_per_row * rows_per_file);
    float **rows = (float**) malloc(sizeof(float*) * rows_per_file);
    for (int ix = 0; ix < rows_per_file; ++ix){
        rows[ix] = (float*) malloc(sizeof(float) * 3);
    }


    fseek(file, 0, SEEK_SET);
    fread(cells, sizeof(char), char_per_row * rows_per_file, file);
    //#pragma omp parallel for
    // for (size_t i = 0; i < rows_per_file; i += rows_per_block) {
        
    //     fseek(file, rows_per_block*char_per_row, SEEK_CUR);
    //     fread(cells, sizeof(char), char_per_row * rows_per_file, file);

        
    for (int ix = 0; ix < rows_per_file; ++ix) {
        char temp[8];   //Borde vi använda 3 olika temp så att operationerna inte behöver ske efter varandra?

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
    // }
    fclose(file);

    // Calculate the distances
    for (int ix = 0; ix < rows_per_file; ++ix){
        for (int jx = ix + 1; jx < rows_per_file; ++jx){
            float dist = distance(rows[ix], rows[jx]) * 100;
            dist = round(dist);
            int dist_int = (int) (dist);
            result[dist_int] += 1;
        }
    }

    // Initialize the random number generator
    srand(time(NULL));
    int ix = rand() % (rows_per_file + 1);
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