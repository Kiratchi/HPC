#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>


float compute_distance(float c_1[3], float c_2[3]);
int determine_thread_count(int argc, char const *argv[]);
int count_file_lines(FILE *file);
void parse_file_to_rows(FILE *file, float **rows, int rows_per_file);
void calculate_distance_frequencies(float **rows, int *result, int rows_per_file);

static const int rows_per_block = 1000;

static const int char_per_row = 24;
static const unsigned int max_distance = 3465; // from sqrt(20^2*3)

int main(int argc, char const *argv[]) {

    omp_set_num_threads(determine_thread_count(argc, argv));
    
    FILE *file = fopen("cells_1e4", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file cells!\n");
        return 1;
    }

    int rows_per_file = count_file_lines(file);

    float **rows = (float**) malloc(sizeof(float*) * rows_per_file);
    for (int ix = 0; ix < rows_per_file; ++ix) {
        rows[ix] = (float*) malloc(sizeof(float) * 3);
    }
    parse_file_to_rows(file, rows, rows_per_file);
    fclose(file);

    int *result = (int*) malloc(sizeof(int) * max_distance); // 4*3465
    calculate_distance_frequencies(rows, result, rows_per_file);
    free(rows);

    for (int ix = 0; ix < max_distance; ++ix) {
        if (result[ix] != 0){
            printf("%02d.%02d %d\n", ix/100, ix%100, result[ix]);
        }
    }
    
    free(result);

    return 0;
}


int determine_thread_count(int argc, char const *argv[]){
     // Read arguments from command line
    if (argc != 2){
        fprintf(stderr, "Usage: %s -tT where T is number of threads\n", argv[0]);
        exit(1);
    }

    int T = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int value = atoi(&argv[i][2]);  
            if (argv[i][1] == 't') {
                T = value;
            } else {
                fprintf(stderr, "Unknown option: %s\n, should be -t", argv[i]);
                exit(1);
            }
        } else {
            fprintf(stderr, "Unknown option: %s\n, should start with -", argv[i]);
            exit(1);
        }
    }
    return T;
}

int count_file_lines(FILE *file){
    fseek(file, 0, SEEK_END);
    int rows_per_file = ftell(file) / char_per_row;
    // int last_block_size;            // Lägg in det här senare!!! 
    if (rows_per_file % rows_per_block != 0) {
        fprintf(stderr, "Number of rows not divisible by %i\n", rows_per_block);
        exit(1);
    }
    return rows_per_file;
}

void parse_file_to_rows(FILE *file, float **rows, int rows_per_file){
    
    char *cells = (char*) malloc(sizeof(char) * char_per_row * rows_per_file);
    fseek(file, 0, SEEK_SET);
    
    // #pragma omp parallel for //reduction( + : rows[:max_distance][:2] )
    for (int jx = 0; jx < rows_per_file; jx += rows_per_block) {
        
        //#pragma omp atomic
        fseek(file, jx*char_per_row, SEEK_SET);
        fread(cells, sizeof(char), char_per_row * rows_per_block, file);
        
        for (int ix = 0; ix < rows_per_block; ++ix) {
            sscanf(cells + ix*char_per_row, "%f %f %f", &rows[ix+jx][0], &rows[ix+jx][1], &rows[ix+jx][2]);      
        }

    }
    free(cells);
}

void calculate_distance_frequencies(float **rows, int *result, int rows_per_file){
    #pragma omp parallel for reduction( + : result[:max_distance] )
    for (int ix = 0; ix < rows_per_file; ++ix) {
        for (int jx = ix + 1; jx < rows_per_file; ++jx){
            float dist = compute_distance(rows[ix], rows[jx]) * 100;
            dist = round(dist);
            int dist_int = (int) (dist);
            result[dist_int] += 1;
        }
    }
}

float compute_distance(float c_1[3], float c_2[3]){

    float dist_1 = (c_1[0]-c_2[0]);
    float dist_2 = (c_1[1]-c_2[1]);
    float dist_3 = (c_1[2]-c_2[2]);

    float dist = sqrtf(dist_1 * dist_1 + dist_2 * dist_2 + dist_3 * dist_3);
    
    return dist;
}
