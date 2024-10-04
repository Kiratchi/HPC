#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>


static inline int determine_thread_count(int argc, char const *argv[]);
static inline float** allocate_rows(int rows_per_file);
static inline void free_rows(float **rows, int rows_per_file);
static inline int* allocate_result();
static inline int count_file_lines(FILE *file);
static inline void parse_file_to_rows(FILE *file, float **rows, int rows_per_file);
static inline short string_to_point(char *cells, int index);
static inline void calculate_distance_frequencies(float **rows, int *result, int rows_per_file);
static inline float compute_distance_index(const float *c_1, const float *c_2);
static inline void print_result(int *result);

static const int rows_per_block = 2;
static const int char_per_row = 24;
static const unsigned int max_distance = 3465; // from sqrt(20^2*3)

int main(int argc, char const *argv[]) {

    omp_set_num_threads(determine_thread_count(argc, argv));
    
    FILE *file = fopen("cells", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file cells!\n");
        return 1;
    }

    int rows_per_file = count_file_lines(file);
    
    float **rows = allocate_rows(rows_per_file);
    parse_file_to_rows(file, rows, rows_per_file);
    fclose(file);
    
    int *result = allocate_result();
    calculate_distance_frequencies(rows, result, rows_per_file);
    free_rows(rows, rows_per_file);

    print_result(result);
    
    free(result);

    return 0;
}


static inline int determine_thread_count(int argc, char const *argv[]){
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

static inline int count_file_lines(FILE *file){
    fseek(file, 0, SEEK_END);
    int rows_per_file = ftell(file) / char_per_row;
    //int extra_block_size = rows_per_file % rows_per_block;            // Lägg in det här senare!!! 
    if (rows_per_file % rows_per_block != 0) {
        fprintf(stderr, "Number of rows not divisible by %i\n", rows_per_block);
        //printf("Extra block with %i rows created", extra_block_size);
        exit(1);
    }
    return rows_per_file;
}

static inline float** allocate_rows(int rows_per_file){
    float **rows = (float**) malloc(sizeof(float*) * rows_per_file);
    if (rows == NULL){
        fprintf(stderr, "Memory could not be allocated for rows");
        exit(1);
    }
    for (int ix = 0; ix < rows_per_file; ++ix) {
        rows[ix] = (float*) malloc(sizeof(float) * 3);
        if (rows[ix] == NULL) {
            fprintf(stderr, "Memory could not be allocated for row %i",ix);
            exit(1);
        }
    }
    return rows;
}

static inline void free_rows(float **rows, int rows_per_file){
    for (int ix = 0; ix < rows_per_file; ++ix) {
        free(rows[ix]);
    }
    free(rows);
}

static inline int* allocate_result(){
    //int *result = (int*) malloc(sizeof(int) * max_distance); // 4*3465
    int *result = (int*) calloc(max_distance, sizeof(int)); // 4*3465
    
    if (result == NULL){
        fprintf(stderr, "Memory could not be allocated for result");
        exit(1);
    }
    return result;
}

static inline void parse_file_to_rows(FILE *file, float **rows, int rows_per_file){
    
    char *cells = (char*) malloc(sizeof(char) * char_per_row * rows_per_file);
    if (cells == NULL){
        fprintf(stderr, "Memory could not be allocated for cells");
        exit(1);
    }
    fseek(file, 0, SEEK_SET);
    
    for (int jx = 0; jx < rows_per_file; jx += rows_per_block) {
        
        fseek(file, jx*char_per_row, SEEK_SET);
        fread(cells, sizeof(char), char_per_row * rows_per_block, file);
        
        # pragma omp parallel for
        for (int ix = 0; ix < rows_per_block; ++ix) {
            //sscanf(cells + ix*char_per_row, "%f %f %f", &rows[ix+jx][0], &rows[ix+jx][1], &rows[ix+jx][2]);      
            
            int cells_row_start_index = ix*char_per_row;
    
            rows[ix+jx][0] = ((float) string_to_point(cells, cells_row_start_index)) /1000;
            rows[ix+jx][1] = ((float) string_to_point(cells, cells_row_start_index+8)) /1000;
            rows[ix+jx][2] = ((float) string_to_point(cells, cells_row_start_index+16)) /1000;
        }

    }
    free(cells);
}

static inline short string_to_point(char *cells, int index){
    return (44 - cells[index]  ) * (
                    10000*(cells[index+1]-48)
                    + 1000* (cells[index+2]-48)
                    +  100* (cells[index+4]-48)
                    +   10* (cells[index+5]-48)
                    +       (cells[index+6]-48));
}

static inline void calculate_distance_frequencies(float **rows, int *result, int rows_per_file){
    # pragma omp parallel for reduction( + : result[:max_distance] )
    for (int ix = 0; ix < rows_per_file; ++ix) {
        for (int jx = ix + 1; jx < rows_per_file; ++jx){
            int dist = (int) compute_distance_index(rows[ix], rows[jx]);
            result[dist] += 1;
        }
    }
}

static inline float compute_distance_index(const float* c_1, const float* c_2){

    float dist_1 = (c_1[0]-c_2[0]);
    float dist_2 = (c_1[1]-c_2[1]);
    float dist_3 = (c_1[2]-c_2[2]);

    float dist = sqrtf(dist_1 * dist_1 + dist_2 * dist_2 + dist_3 * dist_3) * 100 + 0.5;
    
    return dist;
}

static inline void print_result(int *result){
    for (int ix = 0; ix < max_distance; ++ix) {
        if (result[ix] != 0){
            fprintf(stdout, "%02d.%02d %d\n", ix/100, ix%100, result[ix]);
        }
    }
}