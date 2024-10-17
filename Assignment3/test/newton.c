#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <threads.h>
#include <stdint.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Maximum number of iterations
#define MAX_ITER 128
// Convergence threshold
#define CONVERGENCE_THRESHOLD 1e-6
// Divergence threshold
#define DIVERGENCE_THRESHOLD 1e10
// Image bounds
#define X_MIN -2.0
#define X_MAX 2.0
#define Y_MIN -2.0
#define Y_MAX 2.0

// Structure to hold command-line arguments
typedef struct {
    int num_threads;  // Number of threads (-t)
    int image_size;   // Image size (-l)
    int degree;       // Degree of the polynomial (d)
} cmd_args_t;

// Structure for compute thread arguments
typedef struct {
    int thread_id;
    int num_threads;
    int image_size;
    int degree;
    uint8_t **attractors;
    uint8_t **convergences;
    double complex *roots;
} compute_thread_args_t;

// Function prototypes
void parse_command_line(int argc, char *argv[], cmd_args_t *args);
int compute_thread_function(void *args);
void compute_roots(int degree, double complex *roots);
int find_closest_root(double complex z, double complex *roots, int degree);
void hsv_to_rgb(double h, double s, double v, uint8_t *r, uint8_t *g, uint8_t *b);

int main(int argc, char *argv[]) {
    cmd_args_t cmd_args;
    parse_command_line(argc, argv, &cmd_args);

    int num_threads = cmd_args.num_threads;
    int image_size = cmd_args.image_size;
    int degree = cmd_args.degree;

    // Allocate memory for attractors and convergences arrays
    uint8_t **attractors = malloc(image_size * sizeof(uint8_t *));
    uint8_t **convergences = malloc(image_size * sizeof(uint8_t *));
    if (!attractors || !convergences) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < image_size; ++i) {
        attractors[i] = malloc(image_size * sizeof(uint8_t));
        convergences[i] = malloc(image_size * sizeof(uint8_t));
        if (!attractors[i] || !convergences[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }

    // Precompute roots of unity
    double complex roots[degree];
    compute_roots(degree, roots);

    // Create compute threads
    thrd_t compute_threads[num_threads];
    compute_thread_args_t compute_args[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        compute_args[i].thread_id = i;
        compute_args[i].num_threads = num_threads;
        compute_args[i].image_size = image_size;
        compute_args[i].degree = degree;
        compute_args[i].attractors = attractors;
        compute_args[i].convergences = convergences;
        compute_args[i].roots = roots;

        if (thrd_create(&compute_threads[i], compute_thread_function, &compute_args[i]) != thrd_success) {
            fprintf(stderr, "Failed to create compute thread %d\n", i);
            exit(1);
        }
    }

    // Wait for compute threads to complete
    for (int i = 0; i < num_threads; ++i) {
        thrd_join(compute_threads[i], NULL);
    }

    // Write output files
    // Open files
    char attractors_filename[256];
    char convergences_filename[256];
    snprintf(attractors_filename, sizeof(attractors_filename), "newton_attractors_x%d.ppm", degree);
    snprintf(convergences_filename, sizeof(convergences_filename), "newton_convergence_x%d.ppm", degree);

    FILE *attractors_file = fopen(attractors_filename, "w");
    FILE *convergences_file = fopen(convergences_filename, "w");
    if (!attractors_file || !convergences_file) {
        fprintf(stderr, "Failed to open output files\n");
        exit(1);
    }

    // Write headers
    fprintf(attractors_file, "P3\n%d %d\n255\n", image_size, image_size);
    fprintf(convergences_file, "P3\n%d %d\n255\n", image_size, image_size);

    // Prepare colors for roots
    uint8_t root_colors[degree + 1][3]; // Extra color for divergence
    for (int i = 0; i < degree; ++i) {
        double hue = 360.0 * i / degree;
        hsv_to_rgb(hue, 1.0, 1.0, &root_colors[i][0], &root_colors[i][1], &root_colors[i][2]);
    }
    // Divergence color (black)
    root_colors[degree][0] = 0;
    root_colors[degree][1] = 0;
    root_colors[degree][2] = 0;

    // Write image data
    for (int y = 0; y < image_size; ++y) {
        // Attractors image
        for (int x = 0; x < image_size; ++x) {
            int idx = attractors[y][x];
            uint8_t r = root_colors[idx][0];
            uint8_t g = root_colors[idx][1];
            uint8_t b = root_colors[idx][2];
            fprintf(attractors_file, "%d %d %d ", r, g, b);
        }
        fprintf(attractors_file, "\n");

        // Convergences image
        for (int x = 0; x < image_size; ++x) {
            uint8_t val = (uint8_t)(255 - (convergences[y][x] * 255 / 50)); // Map iterations to grayscale
            fprintf(convergences_file, "%d %d %d ", val, val, val);
        }
        fprintf(convergences_file, "\n");
    }

    fclose(attractors_file);
    fclose(convergences_file);

    // Clean up
    for (int i = 0; i < image_size; ++i) {
        free(attractors[i]);
        free(convergences[i]);
    }
    free(attractors);
    free(convergences);

    return 0;
}

// Function to parse command-line arguments
void parse_command_line(int argc, char *argv[], cmd_args_t *args) {
    args->num_threads = 1; // Default to 1 thread
    args->image_size = 1000; // Default image size
    args->degree = 0; // Must be provided

    if (argc < 2) {
        fprintf(stderr, "Usage: %s -tT -lL d\n", argv[0]);
        exit(1);
    }

    // Parse options
    for (int i = 1; i < argc - 1; ++i) {
        if (strncmp(argv[i], "-t", 2) == 0) {
            args->num_threads = atoi(argv[i] + 2);
        } else if (strncmp(argv[i], "-l", 2) == 0) {
            args->image_size = atoi(argv[i] + 2);
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            exit(1);
        }
    }

    // Parse degree
    args->degree = atoi(argv[argc - 1]);
    if (args->degree < 1) {
        fprintf(stderr, "Degree must be at least 1\n");
        exit(1);
    }
}

// Function to compute roots of unity
void compute_roots(int degree, double complex *roots) {
    for (int k = 0; k < degree; ++k) {
        double angle = 2.0 * M_PI * k / degree;
        roots[k] = cos(angle) + sin(angle) * I;
    }
}

// Function to find the closest root
int find_closest_root(double complex z, double complex *roots, int degree) {
    int index = -1;
    double min_dist = INFINITY;
    for (int i = 0; i < degree; ++i) {
        double dist = cabs(z - roots[i]);
        if (dist < min_dist) {
            min_dist = dist;
            index = i;
        }
    }
    return index;
}

// Compute thread function
int compute_thread_function(void *args) {
    compute_thread_args_t *ct_args = (compute_thread_args_t *)args;
    int thread_id = ct_args->thread_id;
    int num_threads = ct_args->num_threads;
    int image_size = ct_args->image_size;
    int degree = ct_args->degree;
    uint8_t **attractors = ct_args->attractors;
    uint8_t **convergences = ct_args->convergences;
    double complex *roots = ct_args->roots;

    double delta_x = (X_MAX - X_MIN) / (image_size - 1);
    double delta_y = (Y_MAX - Y_MIN) / (image_size - 1);

    for (int y = thread_id; y < image_size; y += num_threads) {
        for (int x = 0; x < image_size; ++x) {
            // Map pixel to complex plane
            double real = X_MIN + x * delta_x;
            double imag = Y_MAX - y * delta_y; // Y axis is inverted
            double complex z = real + imag * I;

            int iteration = 0;
            int converged = 0;
            int root_index = -1;

            double complex z_pow = z; // z^1
            double complex z_pow_minus_one = 1.0; // z^0

            // Precompute powers of z
            double complex z_powers[degree]; // Store z^1 to z^degree
            z_powers[0] = z_pow;
            for (int i = 1; i < degree; ++i) {
                z_pow *= z;
                z_powers[i] = z_pow;
            }

            while (iteration < MAX_ITER) {
                // Compute f(z) = z^d - 1
                double complex f = z_powers[degree - 1] - 1.0;
                // Compute f'(z) = d * z^(d-1)
                double complex f_prime = degree * z_powers[degree - 2];

                // Avoid division by zero
                if (cabs(f_prime) == 0.0) {
                    break;
                }

                // Newton's iteration
                z = z - f / f_prime;

                // Update powers of z
                z_pow = z;
                z_powers[0] = z_pow;
                for (int i = 1; i < degree; ++i) {
                    z_pow *= z;
                    z_powers[i] = z_pow;
                }

                // Check for convergence
                double mag = cabs(z);
                if (mag > DIVERGENCE_THRESHOLD) {
                    break;
                }

                double err = cabs(f);
                if (err < CONVERGENCE_THRESHOLD) {
                    root_index = find_closest_root(z, roots, degree);
                    converged = 1;
                    break;
                }

                iteration++;
            }

            // Handle divergence
            if (!converged) {
                root_index = degree; // Use degree as index for divergence
                iteration = MAX_ITER;
            }

            // Store results
            attractors[y][x] = (uint8_t)root_index;
            convergences[y][x] = (uint8_t)(iteration > 50 ? 50 : iteration); // Cap at 50
        }
    }

    return 0;
}

// Function to convert HSV to RGB
void hsv_to_rgb(double h, double s, double v, uint8_t *r, uint8_t *g, uint8_t *b) {
    double c = v * s;
    double h_prime = fmod(h / 60.0, 6);
    if (h_prime < 0) h_prime += 6;
    double x = c * (1 - fabs(fmod(h_prime, 2) - 1));
    double m = v - c;

    double r_p, g_p, b_p;
    if (0 <= h_prime && h_prime < 1) {
        r_p = c;
        g_p = x;
        b_p = 0;
    } else if (1 <= h_prime && h_prime < 2) {
        r_p = x;
        g_p = c;
        b_p = 0;
    } else if (2 <= h_prime && h_prime < 3) {
        r_p = 0;
        g_p = c;
        b_p = x;
    } else if (3 <= h_prime && h_prime < 4) {
        r_p = 0;
        g_p = x;
        b_p = c;
    } else if (4 <= h_prime && h_prime < 5) {
        r_p = x;
        g_p = 0;
        b_p = c;
    } else {
        r_p = c;
        g_p = 0;
        b_p = x;
    }

    *r = (uint8_t)((r_p + m) * 255);
    *g = (uint8_t)((g_p + m) * 255);
    *b = (uint8_t)((b_p + m) * 255);
}
