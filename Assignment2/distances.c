#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>

static inline int determine_thread_count(int argc, char const *argv[]);
static inline FILE* open_file();
static inline float* allocate_rows(int nr_rows);
static inline int* allocate_result();
static inline int count_file_lines(FILE *file);
static inline void parse_file_to_rows(FILE *file, float *rows, int nr_rows, int initial_row);
static inline short string_to_point(char *cells, int index);
static inline void calculate_distance_frequencies(float *primary_rows,float *secondary_rows, int *result, int primary_rows_per_block,  int secondary_rows_per_block);
static inline void calculate_same_distance_frequencies(float *primary_rows, int *result, int nr_rows);
static inline int compute_distance_index(const float *c_1, const float *c_2);
static inline void print_result(int *result);

static const int rows_per_block = 1000;
static const int char_per_row = 24;
static const unsigned int max_distance = 3465; // from sqrt(20^2*3)

int main(int argc, char const *argv[]) {

    omp_set_num_threads(determine_thread_count(argc, argv));
    FILE *file = open_file();
    int rows_per_file = count_file_lines(file);
    int *result = allocate_result();
    
    float *primary_rows = allocate_rows(rows_per_block);
    float *secondary_rows = allocate_rows(rows_per_block);
    
    for (int primary_initial_row = 0; primary_initial_row < rows_per_file; primary_initial_row += rows_per_block){
        int primary_rows_per_block = primary_initial_row + rows_per_block < rows_per_file ? rows_per_block : rows_per_file - primary_initial_row;
        
        parse_file_to_rows(file, primary_rows, primary_rows_per_block, primary_initial_row);
        calculate_same_distance_frequencies(primary_rows, result, primary_rows_per_block);
        
        for (int secondary_initial_row = primary_initial_row + primary_rows_per_block; secondary_initial_row < rows_per_file; secondary_initial_row += rows_per_block){
            int secondary_rows_per_block = secondary_initial_row + rows_per_block < rows_per_file ? rows_per_block : rows_per_file - secondary_initial_row;
            
            parse_file_to_rows(file, secondary_rows, primary_rows_per_block,secondary_initial_row);
            calculate_distance_frequencies(primary_rows, secondary_rows ,result, primary_rows_per_block, secondary_rows_per_block);
            
        }  
    }
    fclose(file);
    free(primary_rows);
    free(secondary_rows);
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

static inline FILE* open_file(){
    FILE *file = fopen("cells_1e5", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file cells!\n");
        exit(1);
    }
    return file;
}

static inline int count_file_lines(FILE *file){
    fseek(file, 0, SEEK_END);
    int rows_per_file = ftell(file) / char_per_row;
    fseek(file, 0, SEEK_SET);
    return rows_per_file;
}

static inline float* allocate_rows(int nr_rows){
    float *rows = (float*) malloc(3 * sizeof(float) * nr_rows);
    if (rows == NULL){
        fprintf(stderr, "Memory could not be allocated for rows");
        exit(1);
    }
    return rows;
}


static inline int* allocate_result(){
    int *result = (int*) calloc(max_distance, sizeof(int));
    if (result == NULL){
        fprintf(stderr, "Memory could not be allocated for result");
        exit(1);
    }
    return result;
}

static inline void parse_file_to_rows(FILE *file, float *rows, int nr_rows, int initial_row){
    char *file_content = (char*) malloc(sizeof(char) * char_per_row * nr_rows); //Utan malloc??
    if (file_content == NULL){
        fprintf(stderr, "Memory could not be allocated for file_content");
        exit(1);
    }
    fseek(file, initial_row*char_per_row, SEEK_SET);
    fread(file_content, sizeof(char), char_per_row * nr_rows, file);  
    # pragma omp parallel for
    for (int jx = 0; jx < nr_rows; ++jx) {   
        int cells_row_start_index = jx*char_per_row;
        rows[3*jx+0] = ((float) string_to_point(file_content, cells_row_start_index)) /1000;
        rows[3*jx+1] = ((float) string_to_point(file_content, cells_row_start_index+8)) /1000;
        rows[3*jx+2] = ((float) string_to_point(file_content, cells_row_start_index+16)) /1000;
    }
    free(file_content);
}

static inline short string_to_point(char *cells, int index){
    return (44 - cells[index]  ) * (
                     10000* (cells[index+1]-'0')
                    + 1000* (cells[index+2]-'0')
                    +  100* (cells[index+4]-'0')
                    +   10* (cells[index+5]-'0')
                    +       (cells[index+6]-'0'));
}

static inline void calculate_same_distance_frequencies(float *primary_rows, int *result, int nr_rows){
    # pragma omp parallel for reduction( + : result[:max_distance] )
    for (int ix = 0; ix < nr_rows; ++ix) {
        for (int jx = ix + 1; jx < nr_rows; ++jx){
            int dist = compute_distance_index(&primary_rows[3*ix], &primary_rows[3*jx]);
            result[dist] += 1;
        }
    }
}

static inline void calculate_distance_frequencies(float *primary_rows,float *secondary_rows, int *result, int primary_rows_per_block,  int secondary_rows_per_block){
    # pragma omp parallel for reduction( + : result[:max_distance] )
    for (int ix = 0; ix < primary_rows_per_block; ++ix) {
        for (int jx = 0; jx < secondary_rows_per_block; ++jx){
            int dist = (int) compute_distance_index(&primary_rows[3*ix], &secondary_rows[3*jx]);
            result[dist] += 1;
        }
    }
}

static inline int compute_distance_index(const float* c_1, const float* c_2){
    float dist_1 = (c_1[0]-c_2[0]);
    float dist_2 = (c_1[1]-c_2[1]);
    float dist_3 = (c_1[2]-c_2[2]);
    int dist = (int) (sqrtf(dist_1 * dist_1 + dist_2 * dist_2 + dist_3 * dist_3) * 100.0f + 0.5f);
    return dist;
}

static inline void print_result(int *result){
    for (int ix = 0; ix < max_distance; ++ix) {
        if (result[ix] != 0){
            fprintf(stdout, "%02d.%02d %d\n", ix/100, ix%100, result[ix]);
        }
    }
}