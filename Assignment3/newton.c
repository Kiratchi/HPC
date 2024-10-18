#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

// GLOBAL VARIABLES
int T;
int L;
int power;

typedef struct {
  int x;
  int y;
} TwoIntegers;

typedef struct {
  int val;
  char pad[60]; // cacheline - sizeof(int)
} int_padded;

typedef struct {
  int **attractors;
  int **convergences;
  complex float *exact_roots;
  int istep;  // number of threads
  int sz;     // numebr of lines
  int tx;     // thread index
  mtx_t *mtx; // mutex
  cnd_t *cnd; // condition
  int_padded *status;
} thrd_arg_compute_t; // Was thrd_info_t

typedef struct {
  int **attractors;
  int **convergences;
  int sz;     // number of lines
  int nthrds; // number of threads
  mtx_t *mtx; // mutex
  cnd_t *cnd; // condition
  int_padded *status;
} thrd_arg_write_t; // Was thrd_info_check_t

const char grey_str[][12] = {
    "000 000 000 ", "001 001 001 ", "002 002 002 ", "003 003 003 ",
    "004 004 004 ", "005 005 005 ", "006 006 006 ", "007 007 007 ",
    "008 008 008 ", "009 009 009 ", "010 010 010 ", "011 011 011 ",
    "012 012 012 ", "013 013 013 ", "014 014 014 ", "015 015 015 ",
    "016 016 016 ", "017 017 017 ", "018 018 018 ", "019 019 019 ",
    "020 020 020 ", "021 021 021 ", "022 022 022 ", "023 023 023 ",
    "024 024 024 ", "025 025 025 ", "026 026 026 ", "027 027 027 ",
    "028 028 028 ", "029 029 029 ", "030 030 030 ", "031 031 031 ",
    "032 032 032 ", "033 033 033 ", "034 034 034 ", "035 035 035 ",
    "036 036 036 ", "037 037 037 ", "038 038 038 ", "039 039 039 ",
    "040 040 040 ", "041 041 041 ", "042 042 042 ", "043 043 043 ",
    "044 044 044 ", "045 045 045 ", "046 046 046 ", "047 047 047 ",
    "048 048 048 ", "049 049 049 ", "050 050 050 ", "051 051 051 ",
    "052 052 052 ", "053 053 053 ", "054 054 054 ", "055 055 055 ",
    "056 056 056 ", "057 057 057 ", "058 058 058 ", "059 059 059 ",
    "060 060 060 ", "061 061 061 ", "062 062 062 ", "063 063 063 ",
    "064 064 064 ", "065 065 065 ", "066 066 066 ", "067 067 067 ",
    "068 068 068 ", "069 069 069 ", "070 070 070 ", "071 071 071 ",
    "072 072 072 ", "073 073 073 ", "074 074 074 ", "075 075 075 ",
    "076 076 076 ", "077 077 077 ", "078 078 078 ", "079 079 079 ",
    "080 080 080 ", "081 081 081 ", "082 082 082 ", "083 083 083 ",
    "084 084 084 ", "085 085 085 ", "086 086 086 ", "087 087 087 ",
    "088 088 088 ", "089 089 089 ", "090 090 090 ", "091 091 091 ",
    "092 092 092 ", "093 093 093 ", "094 094 094 ", "095 095 095 ",
    "096 096 096 ", "097 097 097 ", "098 098 098 ", "099 099 099 ",
    "100 100 100 ", "101 101 101 ", "102 102 102 ", "103 103 103 ",
    "104 104 104 ", "105 105 105 ", "106 106 106 ", "107 107 107 ",
    "108 108 108 ", "109 109 109 ", "110 110 110 ", "111 111 111 ",
    "112 112 112 ", "113 113 113 ", "114 114 114 ", "115 115 115 ",
    "116 116 116 ", "117 117 117 ", "118 118 118 ", "119 119 119 ",
    "120 120 120 ", "121 121 121 ", "122 122 122 ", "123 123 123 ",
    "124 124 124 ", "125 125 125 ", "126 126 126 ", "127 127 127 ",
    "128 128 128 "};

const char colour_str[][12] = {
    "015 006 033 ", // Dark Purple
    "042 007 054 ", // Purple
    "068 017 053 ", // Reddish Purple
    "093 027 042 ", // Red-Orange
    "115 044 028 ", // Bright Orange
    "125 067 019 ", // Orange-Yellow
    "126 095 024 ", // Bright Yellow
    "124 121 087 ", // Pale Yellow
    "128 128 128 ", // White (or light)
};

static inline void determine_command_line(int argc, char const *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "Usage: %s -tT -lL X where T is threads, L is pixels, X is power\n",
            argv[0]);
    exit(1);
  }
  for (int i = 1; i < argc - 1; i++) {
    if (argv[i][0] == '-') {
      int value = atoi(&argv[i][2]);
      if (argv[i][1] == 't') {
        T = value;
      } else if (argv[i][1] == 'l') {
        L = value;
      } else {
        fprintf(stderr, "Unknown option: %s\n, should be -t, -l", argv[i]);
        exit(1);
      }
    } else {
      fprintf(stderr, "Unknown option: %s\n, should start with -", argv[i]);
      exit(1);
    }
  }
  power = atoi(argv[3]);
}

// Function to calculate e^(i*pi*k/K) for k in range(0, K)
static inline complex float *find_exact_roots(complex float *exact_roots) {
  // Loop to calculate e^(i*pi*k/K) for each k
  for (int k = 0; k < power; ++k) {
    float angle =
        2 * 3.14159265358979 * k / power; // Calculate the angle (pi * k / K)
    exact_roots[k] =
        cosf(angle) + I * sinf(angle); // Euler's formula: e^(i * angle)
  }
  return exact_roots;
}

static inline complex float calculate_next_iteration(const float complex z) {
  complex float z_new;

  switch (power) {
  case 1:
    z_new = 1.0f;
    break;
  case 2:
    z_new = z - (1.0f/2.0f) * (z - (1.0f / z));
    break;
  case 3: {
    complex float z2 = z * z;
    z_new = z - (1.0f/3.0f) * (z - (1.0f / (z2)));
    break;
  }
  case 4: {
    complex float z2 = z * z;
    z_new = z - (1.0f/4.0f) * (z - (1.0f / (z2*z)));
    break;
  }
  case 5: {
    complex float z2 = z * z;
    complex float z4 = z2 * z2;
    z_new = z - (1.0f/5.0f) * (z - (1.0f / z4));
    break;
  }
  case 6: {
    complex float z2 = z * z;
    complex float z4 = z2 * z2;
    z_new = z - (1.0f/6.0f) * (z - (1.0f / (z4*z)));
    break;
  }
  case 7: {
    complex float z2 = z * z;
    complex float z4 = z2 * z2;
    z_new = z - (1.0f/7.0f) * (z - (1.0f / (z4*z2)));
    break;
  }
  case 8: {
    complex float z2 = z * z;
    complex float z4 = z2 * z2;
    z_new = z - (1.0f/8.0f) * (z - (1.0f / (z4*z2*z)));
    break;
  }
  case 9: {
    complex float z2 = z * z;
    complex float z4 = z2 * z2;
    complex float z8 = z4 * z4;
    z_new = z - (1.0f/9.0f) * (z - (1.0f / (z8)));
    break;
  }
  default:
    fprintf(stderr, "unexpected degree\n");
    exit(1);
  }
  return z_new;
}

static inline TwoIntegers calculate_root(const float x_coord, const float y_coord,
                                         complex float *exact_roots) {
  complex float z = x_coord + y_coord * I;
  complex float new_z = z;

  int iter_counter = 0;
  int idx_closest = 0;
  for (int ix = 0; ix < 128; ix++) {

    iter_counter += 1;
    new_z = calculate_next_iteration(z);
    float new_abs_z =
        crealf(new_z) * crealf(new_z) + cimagf(new_z) * cimagf(new_z);

    short int close_to_root = 0;
    
    for (int kx = 0; kx < power; kx++) {
      float real_diff = crealf(new_z - exact_roots[kx]);
      float imag_diff = cimagf(new_z - exact_roots[kx]);
      float dist_squared = (real_diff * real_diff) +
                     (imag_diff * imag_diff);
      if (dist_squared < 1e-6) {
        close_to_root = 1;
        idx_closest = kx;
        break;
      }
    }
    if (close_to_root == 1) {
      break;
    }

    // Checks if too close or too far away
    else if (new_abs_z < 1e-6) {
      break;
    } else if ((crealf(new_z) > 1e10) || (cimagf(new_z) > 1e10) ||
               (crealf(new_z) < -1e10) || (cimagf(new_z) < -1e10)) {
      break;
    }
    z = new_z;
  }

  TwoIntegers output;
  output.x = idx_closest;
  output.y = iter_counter;

  return output;
}

int main_thrd_compute(void *args) {
  const thrd_arg_compute_t *thrd_info = (thrd_arg_compute_t *)args;
  int **attractors = thrd_info->attractors;
  int **convergences = thrd_info->convergences;
  complex float *exact_roots = thrd_info->exact_roots;
  const int istep = thrd_info->istep;
  const int sz = thrd_info->sz;
  const int tx = thrd_info->tx;
  mtx_t *mtx = thrd_info->mtx;
  cnd_t *cnd = thrd_info->cnd;
  int_padded *status = thrd_info->status;

  for (int ix = tx; ix < sz; ix += istep) {
    int *attractors_ix = (int *)malloc(sz * sizeof(int));
    int *convergences_ix = (int *)malloc(sz * sizeof(int));

    for (size_t cx = 0; cx < sz; ++cx) {
      attractors_ix[cx] = 0;
      convergences_ix[cx] = 0;
    }

    for (int jx = 0; jx < sz; ++jx) {
      TwoIntegers counts =
          calculate_root((float) (( (float) jx*4.0f / (float) sz) - 2.0f), (float) (((float) ix*4.0f / (float) sz) - 2.0f) , exact_roots);
      attractors_ix[jx] = counts.x;
      convergences_ix[jx] = counts.y;
    }

    mtx_lock(mtx);
    attractors[ix] = (int *)attractors_ix;
    convergences[ix] = (int *)convergences_ix;
    status[tx].val = ix + istep;
    mtx_unlock(mtx);
    cnd_signal(cnd);
  }

  return 0;
}

int main_thrd_write(void *args) {
  const thrd_arg_write_t *thrd_info = (thrd_arg_write_t *)args;
  int **attractors = thrd_info->attractors;
  int **convergences = thrd_info->convergences;
  const int sz = thrd_info->sz;
  const int nthrds = thrd_info->nthrds;
  mtx_t *mtx = thrd_info->mtx;
  cnd_t *cnd = thrd_info->cnd;
  int_padded *status = thrd_info->status;

  char attr_name[25];
  char conv_name[26];
  sprintf(attr_name, "newton_attractors_x%d.ppm", power);
  sprintf(conv_name, "newton_convergence_x%d.ppm", power);
  FILE *attr_file = fopen(attr_name, "w");
  FILE *conv_file = fopen(conv_name, "w");

  int M = 128;
  int L_length = (int)log10(L) + 1;
  int header_length = 2 * L_length + 9;
  char header[header_length];
  sprintf(header, "P3\n%d %d\n%d\n", L, L, M);
  fwrite(header, sizeof(char), header_length, attr_file);
  fwrite(header, sizeof(char), header_length, conv_file);

  for (int ix = 0, ibnd; ix < sz;) {
    for (mtx_lock(mtx);;) {
      ibnd = sz;
      for (int tx = 0; tx < nthrds; ++tx)
        if (ibnd > status[tx].val)
          ibnd = status[tx].val;
      if (ibnd <= ix)
        cnd_wait(cnd, mtx);
      else {
        mtx_unlock(mtx);
        break;
      }
    }
    for (; ix < ibnd; ++ix) {
      for (int jx = 0; jx < sz; ++jx) {
        fwrite(grey_str[convergences[ix][jx]], sizeof(char), 12, conv_file);
        fwrite(colour_str[attractors[ix][jx]], sizeof(char), 12, attr_file);
      }
      free(attractors[ix]);
      free(convergences[ix]);
    }
  }
  fclose(attr_file);
  fclose(conv_file);
  return 0;
}

int main(int argc, char const *argv[]) {
  determine_command_line(argc, argv);
  printf("T: %d, L: %d, power: %d\n", T, L, power); // Ha kvar denna sålänge!

  complex float exact_roots[power];
  find_exact_roots(exact_roots);

  const int sz = (const int)L;

  float **attractors = (float **)malloc(sz * sizeof(float *));
  int **convergence = (int **)malloc(sz * sizeof(int *));

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

  for (int tx = 0; tx < nthrds; ++tx) {
    thrds_info[tx].attractors = (int **)attractors;
    thrds_info[tx].convergences = (int **)convergence;
    thrds_info[tx].exact_roots = (complex float *)exact_roots;
    thrds_info[tx].istep = nthrds;
    thrds_info[tx].sz = sz;
    thrds_info[tx].tx = tx;
    thrds_info[tx].mtx = &mtx;
    thrds_info[tx].cnd = &cnd;
    thrds_info[tx].status = status;
    status[tx].val = -1;

    int r =
        thrd_create(thrds + tx, main_thrd_compute, (void *)(thrds_info + tx));
    if (r != thrd_success) {
      fprintf(stderr, "failed to create thread\n");
      exit(1);
    }
    thrd_detach(thrds[tx]);
  }

  {
    thrd_info_check.attractors = (int **)attractors;
    thrd_info_check.convergences = (int **)convergence;
    thrd_info_check.sz = sz;
    thrd_info_check.nthrds = nthrds;
    thrd_info_check.mtx = &mtx;
    thrd_info_check.cnd = &cnd;
    thrd_info_check.status = status;

    int r =
        thrd_create(&thrd_check, main_thrd_write, (void *)(&thrd_info_check));
    if (r != thrd_success) {
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
