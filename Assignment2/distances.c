#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>


static inline int determine_thread_count(int argc, char const *argv[]);
static inline FILE* open_file();
static inline float** allocate_rows(int nr_rows);
static inline void free_rows(float **rows, int nr_rows);
static inline int* allocate_result();
static inline void count_file_lines(FILE *file, int *rows_per_file, int *extra_block_size);
static inline void parse_file_to_rows(FILE *file, float **rows, int nr_rows, int initial_row);
static inline short string_to_point(char *cells, int index);
static inline void calculate_distance_frequencies(float **rows, int *result, int rows_per_file);
static inline float compute_distance_index(const float *c_1, const float *c_2);
static inline void print_result(int *result);

static const int rows_per_block = 10;
static const int char_per_row = 24;
static const unsigned int max_distance = 3465; // from sqrt(20^2*3)

int main(int argc, char const *argv[]) {

    omp_set_num_threads(determine_thread_count(argc, argv));
    
    FILE *file = open_file();

    int rows_per_file;
    int extra_block_size;
    count_file_lines(file, &rows_per_file, &extra_block_size);
    
    float **rows = allocate_rows(rows_per_file);
    parse_file_to_rows(file, rows, rows_per_file, 0);
    fclose(file);
    
    int *result = allocate_result();
    calculate_distance_frequencies(rows, result, rows_per_file);
    free_rows(rows, rows_per_file);

    print_result(result);
    
    free(result);


    // VARIANT I BLOCK FRÅN HÄR

    // omp_set_num_threads(determine_thread_count(argc, argv));
    // FILE *file = open_file();
    // int rows_per_file;
    // int extra_block_size;
    // count_file_lines(file, &rows_per_file, &extra_block_size);
    // fclose(file);
    // int *result = allocate_result();
    

    // Håll koll på sista blocket om det blir mindre!
    // for (int initial_row = 0; initial_row < rows_per_file; initial_row += rows_per_block){
    //     float **rows = allocate_rows(rows_per_block);
        
    //     FILE *file = open_file();
    //     parse_file_to_rows(file, rows, rows_per_block,initial_row);
    //     fclose(file);

    //     calculate_distance_frequencies(rows, result, rows_per_block);
    //     free_rows(rows, rows_per_block);
        

    // }
    // print_result(result);
    // free(result);

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

static inline FILE* open_file(){
    FILE *file = fopen("cells", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file cells!\n");
        exit(1);
    }
    return file;
}

static inline void count_file_lines(FILE *file, int *rows_per_file, int *extra_block_size){
    fseek(file, 0, SEEK_END);
    *rows_per_file = ftell(file) / char_per_row;
    *extra_block_size = *rows_per_file % rows_per_block;            // Lägg in det här senare!!! 
    if (*extra_block_size != 0) {
        fprintf(stderr, "Number of rows not divisible by %i\n", rows_per_block);
        //printf("Extra block with %i rows created", extra_block_size);
        exit(1);
    }
    fseek(file, 0, SEEK_SET);
}

static inline float** allocate_rows(int nr_rows){
    float **rows = (float**) malloc(sizeof(float*) * nr_rows);
    if (rows == NULL){
        fprintf(stderr, "Memory could not be allocated for rows");
        exit(1);
    }
    for (int ix = 0; ix < nr_rows; ++ix) {
        rows[ix] = (float*) malloc(sizeof(float) * 3);
        if (rows[ix] == NULL) {
            fprintf(stderr, "Memory could not be allocated for row %i",ix);
            exit(1);
        }
    }
    return rows;
}

static inline void free_rows(float **rows, int nr_rows){
    for (int ix = 0; ix < nr_rows; ++ix) {
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

static inline void parse_file_to_rows(FILE *file, float **rows, int nr_rows, int initial_row){
    
    // Kan vi skippa cells när vi gör saker i blocks?
    char *cells = (char*) malloc(sizeof(char) * char_per_row * nr_rows);
    if (cells == NULL){
        fprintf(stderr, "Memory could not be allocated for cells");
        exit(1);
    }
    fseek(file, 0, SEEK_SET); //Kan skippa om vi är smarta
    
    for (int jx = 0; jx < nr_rows; jx += rows_per_block) {
        
        fseek(file, jx*char_per_row + initial_row, SEEK_SET); //Kan ändra till att bara flytta markören
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

static inline void calculate_distance_frequencies(float **rows, int *result, int nr_rows){
    # pragma omp parallel for reduction( + : result[:max_distance] )
    for (int ix = 0; ix < nr_rows; ++ix) {
        for (int jx = ix + 1; jx < nr_rows; ++jx){
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