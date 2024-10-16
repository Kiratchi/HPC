#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <threads.h>
#include <complex.h>

// GLOBAL VARIABLES
int T;
int L;
int power;

// Mmmm, En glorifierad int
typedef struct
{
    int val;
    char pad[60]; // cacheline - sizeof(int)
} int_padded;


typedef struct
{
    int **attractors;
    int **convergences;
    complex float *complex_roots;
    int istep;  // number of threads
    int sz;     // numebr of lines
    int tx;     // thread index
    mtx_t *mtx; // mutex
    cnd_t *cnd; // condition
    int_padded *status;
} thrd_arg_compute_t; // Was thrd_info_t


typedef struct
{
    int **attractors;
    int **convergences;
    int sz;     // number of lines
    int nthrds; // number of threads
    mtx_t *mtx; // mutex
    cnd_t *cnd; // condition
    int_padded *status;
} thrd_arg_write_t; // Was thrd_info_check_t


static inline void determine_command_line(int argc, char const *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s -tT -lL X where T is threads, L is pixels, X is power\n", argv[0]);
        exit(1);
    }
    for (int i = 1; i < argc - 1; i++)
    {
        if (argv[i][0] == '-')
        {
            int value = atoi(&argv[i][2]);
            if (argv[i][1] == 't')
            {
                T = value;
            }
            else if (argv[i][1] == 'l')
            {
                L = value;
            }
            else
            {
                fprintf(stderr, "Unknown option: %s\n, should be -t, -l", argv[i]);
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "Unknown option: %s\n, should start with -", argv[i]);
            exit(1);
        }
    }
    power = atoi(argv[3]);
}

// Function to calculate e^(i*pi*k/K) for k in range(0, K)
static inline complex float* calculate_exponentials(complex float * result) {
    // Allocate memory for an array to store the complex numbers

    // Loop to calculate e^(i*pi*k/K) for each k
    for (int k = 0; k < power; ++k) {
        float angle = 2*3.14159265358979 * k / power;  // Calculate the angle (pi * k / K)
        result[k] = cosf(angle) + I * sinf(angle);  // Euler's formula: e^(i * angle)
    }

    // return result;
    return result;
}


static inline float complex newton_function(const float complex input_number) {
    return cpow(input_number, power) - 1.0f;
}

static inline float complex newton_derivative(const float complex input_number) {
    return power * cpow(input_number, power-1);
}

static inline float complex calculate_root(const int x_cord, const int y_cord, complex float *roots){
    float complex z = (float)x_cord + (float)y_cord * I;
    float complex new_z = z;
    
    // This could be changed to 50 instead of 128
    for (size_t ix = 0; ix < 128; ix++)
    {
        new_z = z - ( newton_function(z) / newton_derivative(z) );
        
        // char close_to_root = "0";
        // for (size_t k = 0; k < L; k++)
        // {
        //     if (roots[k] - new_z < 0.001) {
        //         close_to_root = "1";
        //     new_z = root;
        // }
        // if (close_to_root == "1"){
        //     break
        // }
        // }
        // else if ( cabsf(new_z) < 0.001 ) {
        //     break;
        // }
        // else if ((crealf(new_z) > powf(10,5)) || (cimagf(new_z) > powf(10,5)))
        // {
        //     break;
        // }
        
        z = new_z;
    }
    
    return new_z; // Return iteration count as well
}


// static inline float newton_function(const float input_number, const int power)

int main_thrd_compute(
    void *args)
{
    const thrd_arg_compute_t *thrd_info = (thrd_arg_compute_t *)args;
    int **attractors = thrd_info->attractors;
    int **convergences = thrd_info->convergences;
    complex float *complex_roots = thrd_info -> complex_roots;
    const int istep = thrd_info->istep;
    const int sz = thrd_info->sz;
    const int tx = thrd_info->tx;
    mtx_t *mtx = thrd_info->mtx;
    cnd_t *cnd = thrd_info->cnd;
    int_padded *status = thrd_info->status;

    for (int ix = tx; ix < sz; ix += istep){
        // We allocate the rows of the result before computing, and free them in another thread.
        int *attractors_ix = (int *)malloc(sz * sizeof(int));
        int *convergences_ix = (int *)malloc(sz * sizeof(int));

        // To guarantee meaningful input values in the writing thread. (from exercise)
        for ( size_t cx = 0; cx < sz; ++cx ) {
            attractors_ix[cx] = 0;
            convergences_ix[cx] = 0;
        }

        // Lägg in Newton-func
        for (int jx = 0; jx < sz; ++jx) {
            // calculate_root(ix,jx, complex_roots);
            attractors_ix[jx] = (int) jx;
            convergences_ix[jx] = (int) jx;
        }

        mtx_lock(mtx);
        attractors[ix] = (int*) attractors_ix;
        convergences[ix] = (int*) convergences_ix;
        status[tx].val = ix + istep;
        mtx_unlock(mtx);
        cnd_signal(cnd);
    }

    return 0;
}


void set_color(int NUMBER, int *COLOUR_RED, int *COLOUR_GREEN, int *COLOUR_BLUE) {
    switch (NUMBER) {
        case 0: // Red
            *COLOUR_RED = 255; *COLOUR_GREEN = 0;   *COLOUR_BLUE = 0;
            break;
        case 1: // Green
            *COLOUR_RED = 0;   *COLOUR_GREEN = 255; *COLOUR_BLUE = 0;
            break;
        case 2: // Blue
            *COLOUR_RED = 0;   *COLOUR_GREEN = 0;   *COLOUR_BLUE = 255;
            break;
        case 3: // Cyan
            *COLOUR_RED = 0;   *COLOUR_GREEN = 255; *COLOUR_BLUE = 255;
            break;
        case 4: // Magenta
            *COLOUR_RED = 255; *COLOUR_GREEN = 0;   *COLOUR_BLUE = 255;
            break;
        case 5: // Yellow
            *COLOUR_RED = 255; *COLOUR_GREEN = 255; *COLOUR_BLUE = 0;
            break;
        case 6: // Orange
            *COLOUR_RED = 255; *COLOUR_GREEN = 165; *COLOUR_BLUE = 0;
            break;
        case 7: // Purple
            *COLOUR_RED = 128; *COLOUR_GREEN = 0;   *COLOUR_BLUE = 128;
            break;
        case 8: // Brown
            *COLOUR_RED = 165; *COLOUR_GREEN = 42;  *COLOUR_BLUE = 42;
            break;
        case 9: // Lime
            *COLOUR_RED = 50;  *COLOUR_GREEN = 205; *COLOUR_BLUE = 50;
            break;
        default: // Default to black if NUMBER is out of range
            *COLOUR_RED = 0;   *COLOUR_GREEN = 0;   *COLOUR_BLUE = 0;
            break;
    }
}

int main_thrd_write( 
    void *args)
{
    const thrd_arg_write_t *thrd_info = (thrd_arg_write_t *)args;
    int **attractors = thrd_info->attractors;
    int **convergences = thrd_info->convergences;
    const int sz = thrd_info->sz;
    const int nthrds = thrd_info->nthrds;
    mtx_t *mtx = thrd_info->mtx;
    cnd_t *cnd = thrd_info->cnd;
    int_padded *status = thrd_info->status;

    FILE *attr_file = fopen("newton_attractors_xd.ppm", "w");
    FILE *conv_file = fopen("newton_convergence_xd.ppm", "w");

    // Write the header of the PPM file
    int M = 255;
    int L_length = (int)log10(L) + 1;
    int header_length = L_length + 9;
    char header[header_length];
    sprintf(header, "P3\n%d %d\n%d\n", L, L, M);
    fwrite(header, sizeof(char), header_length, attr_file);
    fwrite(header, sizeof(char), header_length, conv_file);
    char grey_str[12]; // Length for "RRR RRR RRR "
    char colour_str[12]; // Length for "RRR GGG BBB "

    // We do not increment ix in this loop, but in the inner one.
    for (int ix = 0, ibnd; ix < sz;)
    {

        // If no new lines are available, we wait.
        for (mtx_lock(mtx); ; )
        {   
            // We extract the minimum of all status variables.
            ibnd = sz;
            for (int tx = 0; tx < nthrds; ++tx)
                if (ibnd > status[tx].val)
                    ibnd = status[tx].val;

            if (ibnd <= ix)
                // Until here the mutex protects status updates, so that if further
                // updates are pending in blocked threads, there will be a subsequent
                // signal.
                cnd_wait(cnd, mtx);
            else
            {
                mtx_unlock(mtx);
                break;
            }

        }

        // HERE WE SHOULD IMPLEMENT THE WRITE_TO_PPM
        fprintf(stderr, "checking until %i\n", ibnd);

        // We do not initialize ix in this loop, but in the outer one.
        for (; ix < ibnd; ++ix){
            for ( int jx = 0; jx < sz; ++jx ){
                int GREY_VALUE = (int) (convergences[ix][jx] * 5);
                sprintf(grey_str, "%03d %03d %03d ", GREY_VALUE, GREY_VALUE, GREY_VALUE); //Gör om med string encoding
                fwrite(grey_str, sizeof(char), 12, conv_file);
                
                int COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE;
                set_color(attractors[ix][jx], &COLOUR_RED, &COLOUR_GREEN, &COLOUR_BLUE);

                sprintf(colour_str, "%03d %03d %03d ", COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE);
                fwrite(colour_str, sizeof(char), 12, attr_file);
            }
            fwrite("\n", sizeof(char), 1, conv_file);
            fwrite("\n", sizeof(char), 1, attr_file);

            // We free the component of attractors and convergnes, SINCE IT WILL NEVER BE USED AGGAAAAAIN!!.
            free(attractors[ix]);
            free(convergences[ix]);
        }
        fclose(attr_file);
        fclose(conv_file);
    }
    return 0;
}



int main(int argc, char const *argv[])
{

    determine_command_line(argc, argv);
    printf("T: %d, L: %d, power: %d\n", T, L, power); // Ha kvar denna sålänge!

    complex float complex_roots[power];
    calculate_exponentials(complex_roots);
    


    
    
    const int sz = (const int) L;

    float **attractors = (float **) malloc(sz * sizeof(float *));
    int **convergence = (int **) malloc(sz * sizeof(int *));



    // The entries of w will be allocated in the computation threads are freed in
    // the check thread.

    const int nthrds = T;
    thrd_t thrds[nthrds];
    thrd_arg_compute_t thrds_info[nthrds];

    thrd_t thrd_check;
    thrd_arg_write_t thrd_info_check;

    mtx_t mtx;
    mtx_init(&mtx, mtx_plain);

    cnd_t cnd;
    cnd_init(&cnd);

    int_padded status[nthrds];

    for (int tx = 0; tx < nthrds; ++tx)
    {
        thrds_info[tx].attractors = (int**) attractors;
        thrds_info[tx].convergences = (int**) convergence;
        thrds_info[tx].istep = nthrds;
        thrds_info[tx].sz = sz;
        thrds_info[tx].tx = tx;
        thrds_info[tx].mtx = &mtx;
        thrds_info[tx].cnd = &cnd;
        thrds_info[tx].status = status;
        status[tx].val = -1;

        int r = thrd_create(thrds + tx, main_thrd_compute, (void *)(thrds_info + tx));
        if (r != thrd_success)
        {
            fprintf(stderr, "failed to create thread\n");
            exit(1);
        }
        thrd_detach(thrds[tx]);
    }

    {
        thrd_info_check.attractors = (int**) attractors;
        thrd_info_check.convergences = (int**) convergence;
        thrd_info_check.sz = sz;
        thrd_info_check.nthrds = nthrds;
        thrd_info_check.mtx = &mtx;
        thrd_info_check.cnd = &cnd;
        // It is important that we have initialize status in the previous for-loop,
        // since it will be consumed by the check threads.
        thrd_info_check.status = status;

        int r = thrd_create(&thrd_check, main_thrd_write, (void *)(&thrd_info_check));
        if (r != thrd_success)
        {
            fprintf(stderr, "failed to create thread\n");
            exit(1);
        }
    }

    {
        int r;
        thrd_join(thrd_check, &r);
    }


    free(attractors);
    free(convergence);

    mtx_destroy(&mtx);
    cnd_destroy(&cnd);

    return 0;
}
