#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>


static inline int determine_thread_count(int argc, char const *argv[]);
static inline short** allocate_rows(int rows_per_file);
static inline void free_rows(short **rows, int rows_per_file);
static inline int* allocate_result();
static inline int count_file_lines(FILE *file);
static inline void parse_file_to_rows(FILE *file, short **rows, int rows_per_file);
static inline short string_to_point(char *cells, int index);
static inline void calculate_distance_frequencies(short **rows, int *result, int rows_per_file);
static inline float compute_distance(const short *c_1, const short *c_2);
static inline void print_result(int *result);

static const int rows_per_block = 1000;

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
    
    short **rows = allocate_rows(rows_per_file);
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

static inline short** allocate_rows(int rows_per_file){
    short **rows = (short**) malloc(sizeof(short*) * rows_per_file);
    if (rows == NULL){
        fprintf(stderr, "Memory could not be allocated for rows");
        exit(1);
    }
    for (int ix = 0; ix < rows_per_file; ++ix) {
        rows[ix] = (short*) malloc(sizeof(short) * 3);
        if (rows[ix] == NULL) {
            fprintf(stderr, "Memory could not be allocated for row %i",ix);
            exit(1);
        }
    }
    return rows;
}

static inline void free_rows(short **rows, int rows_per_file){
    for (int ix = 0; ix < rows_per_file; ++ix) {
        free(rows[ix]);
    }
    free(rows);
}

static inline int* allocate_result(){
    //int *result = (int*) malloc(sizeof(int) * max_distance); // 4*3465
    int *result = calloc( max_distance, sizeof(int)); // 4*3465
    
    if (result == NULL){
        fprintf(stderr, "Memory could not be allocated for result");
        exit(1);
    }
    return result;
}

static inline void parse_file_to_rows(FILE *file, short **rows, int rows_per_file){
    
    char *cells = (char*) malloc(sizeof(char) * char_per_row * rows_per_file);
    if (cells == NULL){
        fprintf(stderr, "Memory could not be allocated for cells");
        exit(1);
    }
    fseek(file, 0, SEEK_SET);
    
    for (int jx = 0; jx < rows_per_file; jx += rows_per_block) {
        
        fseek(file, jx*char_per_row, SEEK_SET);
        fread(cells, sizeof(char), char_per_row * rows_per_block, file);
        
        // for (int i = 0; i < char_per_row * rows_per_block; ++i){
        //     printf("%c", cells[i]);
        // }

        # pragma omp parallel for
        for (int ix = 0; ix < rows_per_block; ++ix) {
            //sscanf(cells + ix*char_per_row, "%f %f %f", &rows[ix+jx][0], &rows[ix+jx][1], &rows[ix+jx][2]);      
            
            int cells_row_start_index = ix*char_per_row;
            
            // short point_1 = string_to_point(cells, cells_row_start_index);
            // short point_2 = string_to_point(cells, cells_row_start_index+8);
            // short point_3 = string_to_point(cells, cells_row_start_index+16);
            // printf("At row %i points are : %i %i %i\n",(ix+jx), point_1, point_2, point_3);

            rows[ix+jx][0] = string_to_point(cells, cells_row_start_index);
            rows[ix+jx][1]= string_to_point(cells, cells_row_start_index+8);
            rows[ix+jx][2] = string_to_point(cells, cells_row_start_index+16);

            // rows[ix+jx][0] = point_1;
            // rows[ix+jx][1] = point_2;
            // rows[ix+jx][2] = point_3;
            //printf("Lyckats skriva i rows\n");
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

static inline void calculate_distance_frequencies(short **rows, int *result, int rows_per_file){
    # pragma omp parallel for reduction( + : result[:max_distance] )
    for (int ix = 0; ix < rows_per_file; ++ix) {
        for (int jx = ix + 1; jx < rows_per_file; ++jx){
            int dist = (int) compute_distance(rows[ix], rows[jx]);  
            //printf("totd: %i\n", dist);
            result[dist] += 1;
        }
    }
}

static inline float compute_distance(const short* c_1, const short* c_2){

    float dist_1 = ((float) (c_1[0]-c_2[0]) )/1000 ;     //Problem om c_1[0] är stort och c_2[0] är litet? Får inte plats i short
    float dist_2 = ((float) (c_1[1]-c_2[1]) )/1000;
    float dist_3 = ((float) (c_1[2]-c_2[2]) )/1000;

    //printf("dist: %f %f %f \n", dist_1, dist_2, dist_3);
    float dist = sqrtf(dist_1 * dist_1 + dist_2 * dist_2 + dist_3 * dist_3)*100+0.5;
    //printf("totd: %f\n", dist);
    return dist;
}

static inline void print_result(int *result){
    for (int ix = 0; ix < max_distance; ++ix) {
        if (result[ix] != 0){
            fprintf(stdout, "%02d.%02d %d\n", ix/100, ix%100, result[ix]);
        }
    }
}