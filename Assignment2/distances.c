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

    // Read arguments from command line
    if (argc != 2){
        fprintf(stderr, "Usage: %s -tT where T is number of threads\n", argv[0]);
        return 1;
    }

    int T = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int value = atoi(&argv[i][2]);  
            if (argv[i][1] == 't') {
                T = value;
            } else {
                fprintf(stderr, "Unknown option: %s\n, should be -t", argv[i]);
                return 1;
            }
        } else {
            fprintf(stderr, "Unknown option: %s\n, should start with -", argv[i]);
            return 1;
        }
    }

    omp_set_num_threads(T);
    

    const int rows_per_block = 5;
    int last_block_size;            // Lägg in det här senare!!! 
    int rows_per_file;
    const int char_per_row = 24;
    const unsigned int max_distance = 3465; // from sqrt(20^2*3)
    int *result = (int*) malloc(sizeof(int) * max_distance); // 4*3465

    FILE *file = fopen("cells_1e4", "r");
    FILE *file = fopen("cells_1e4", "r");
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
    // fread(cells, sizeof(char), char_per_row * rows_per_file, file);
    // #pragma omp parallel for
    for (size_t i = 0; i < rows_per_file; i += rows_per_block) {
        
        fread(cells, sizeof(char), char_per_row * rows_per_block, file);

        for (int ix = 0; ix < rows_per_block; ++ix) {
            // fscanf(file, "%f %f %f", &rows[ix+1][0], &rows[ix+1][1], &rows[ix+1][2]);
            // printf("%f %f %f", rows[ix+1][0], rows[ix+1][1], rows[ix+1][2]);
            
            char temp[8];   //Borde vi använda 3 olika temp så att operationerna inte behöver ske efter varandra?

            strncpy(temp, &cells[ix * char_per_row], 7);
            temp[7] = '\0';
            rows[ix + i][0] = atof(temp);

            strncpy(temp, &cells[ix * char_per_row + 8], 7);
            temp[7] = '\0';
            rows[ix + i][1] = atof(temp);

            strncpy(temp, &cells[ix * char_per_row + 16], 7);
            temp[7] = '\0';
            rows[ix + i][2] = atof(temp);
        }
    }
    fclose(file);

    // for (int i = 0; i < 10 ; i++) {
    //     printf("%f", rows[i][0]);
    //     printf("  %f", rows[i][1]);
    //     printf("  %f\n", rows[i][2]);
    // }


    // Calculate the distances
    // #pragma omp parallel for
    for (int ix = 0; ix < rows_per_file; ++ix){
        # pragma omp parallel for
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