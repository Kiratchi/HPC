#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <threads.h>

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
    float **attractors;
    int **convergences;
    int istep;  // number of threads
    int sz;     // numebr of lines
    int tx;     // thread index
    mtx_t *mtx; // mutex
    cnd_t *cnd; // condition
    int_padded *status;
} thrd_arg_compute_t; // Was thrd_info_t


typedef struct
{
    float **attractors;
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


int main_thrd_compute(
    void *args)
{
    const thrd_arg_compute_t *thrd_info = (thrd_arg_compute_t *)args;
    float **attractors = thrd_info->attractors;
    int **convergences = thrd_info->convergences;
    const int istep = thrd_info->istep;
    const int sz = thrd_info->sz;
    const int tx = thrd_info->tx;
    mtx_t *mtx = thrd_info->mtx;
    cnd_t *cnd = thrd_info->cnd;
    int_padded *status = thrd_info->status;

    for (int ix = tx; ix < sz; ix += istep)
    {
        // We allocate the rows of the result before computing, and free them in another thread.
        float *attractorsix = (float *)malloc(sz * sizeof(float));
        int *convergencesix = (int *)malloc(sz * sizeof(int));

        // Lägg in Newton-func
        for (int jx = 0; jx < sz; ++jx) 
        {
            attractorsix[jx] = 1.0;
            convergencesix[jx] = 0;
        }

        mtx_lock(mtx);
        attractors[ix] = attractorsix;
        convergences[ix] = convergencesix;
        status[tx].val = ix + istep;
        mtx_unlock(mtx);
        cnd_signal(cnd);
    }

    return 0;
}


int main_thrd_write( // Change to main_thrd_write
    void *args)
{
    const thrd_arg_write_t *thrd_info = (thrd_arg_write_t *)args;
    float **attractors = thrd_info->attractors;
    int **convergences = thrd_info->convergences;
    const int sz = thrd_info->sz;
    const int nthrds = thrd_info->nthrds;
    mtx_t *mtx = thrd_info->mtx;
    cnd_t *cnd = thrd_info->cnd;
    int_padded *status = thrd_info->status;

    // const float eps = 1e-1; //Delete??

    // We do not increment ix in this loop, but in the inner one.
    for (int ix = 0, ibnd; ix < sz;)
    {

        // If no new lines are available, we wait.
        for (mtx_lock(mtx);;)
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
        for (; ix < ibnd; ++ix)
        {
            // We only check the last element in w, since we want to illustrate the
            // situation where the check thread completes fast than the computaton
            // threads.
            int jx = sz - 1;
            //   float diff = v[ix][jx] - w[ix][jx] * w[ix][jx];
            //   if ( diff < -eps || diff > eps ) {
            // fprintf(stderr, "incorrect compuation at %i %i: %f %f %f\n",
            // ix, jx, diff, v[ix][jx], w[ix][jx]);
            // This exists the whole program, including all other threads.
            // exit(1);
            //   }

            // We free the component of attractors and convergnes, SINCE IT WILL NEVER BE USED AGGAAAAAIN!!.
            free(attractors[ix]);
            free(convergences[ix]);
        }
    }

    return 0;
}



int main(int argc, char const *argv[])
{

    determine_command_line(argc, argv);
    printf("T: %d, L: %d, power: %d\n", T, L, power); // Ha kvar denna sålänge!

    const int sz = L;

    float **attractors = (float **)malloc(sz * sizeof(float *));
    float **convergence = (float **)malloc(sz * sizeof(float *));

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
        thrds_info[tx].attractors = (float**) attractors;
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
        thrd_info_check.attractors = (float**) attractors;
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
