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

const char grey_str[][13] = {
    "000 000 000\n", "001 001 001\n", "002 002 002\n", "003 003 003\n",
    "004 004 004\n", "005 005 005\n", "006 006 006\n", "007 007 007\n",
    "008 008 008\n", "009 009 009\n", "010 010 010\n", "011 011 011\n",
    "012 012 012\n", "013 013 013\n", "014 014 014\n", "015 015 015\n",
    "016 016 016\n", "017 017 017\n", "018 018 018\n", "019 019 019\n",
    "020 020 020\n", "021 021 021\n", "022 022 022\n", "023 023 023\n",
    "024 024 024\n", "025 025 025\n", "026 026 026\n", "027 027 027\n",
    "028 028 028\n", "029 029 029\n", "030 030 030\n", "031 031 031\n",
    "032 032 032\n", "033 033 033\n", "034 034 034\n", "035 035 035\n",
    "036 036 036\n", "037 037 037\n", "038 038 038\n", "039 039 039\n",
    "040 040 040\n", "041 041 041\n", "042 042 042\n", "043 043 043\n",
    "044 044 044\n", "045 045 045\n", "046 046 046\n", "047 047 047\n",
    "048 048 048\n", "049 049 049\n", "050 050 050\n", "051 051 051\n",
    "052 052 052\n", "053 053 053\n", "054 054 054\n", "055 055 055\n",
    "056 056 056\n", "057 057 057\n", "058 058 058\n", "059 059 059\n",
    "060 060 060\n", "061 061 061\n", "062 062 062\n", "063 063 063\n",
    "064 064 064\n", "065 065 065\n", "066 066 066\n", "067 067 067\n",
    "068 068 068\n", "069 069 069\n", "070 070 070\n", "071 071 071\n",
    "072 072 072\n", "073 073 073\n", "074 074 074\n", "075 075 075\n",
    "076 076 076\n", "077 077 077\n", "078 078 078\n", "079 079 079\n",
    "080 080 080\n", "081 081 081\n", "082 082 082\n", "083 083 083\n",
    "084 084 084\n", "085 085 085\n", "086 086 086\n", "087 087 087\n",
    "088 088 088\n", "089 089 089\n", "090 090 090\n", "091 091 091\n",
    "092 092 092\n", "093 093 093\n", "094 094 094\n", "095 095 095\n",
    "096 096 096\n", "097 097 097\n", "098 098 098\n", "099 099 099\n",
    "100 100 100\n", "101 101 101\n", "102 102 102\n", "103 103 103\n",
    "104 104 104\n", "105 105 105\n", "106 106 106\n", "107 107 107\n",
    "108 108 108\n", "109 109 109\n", "110 110 110\n", "111 111 111\n",
    "112 112 112\n", "113 113 113\n", "114 114 114\n", "115 115 115\n",
    "116 116 116\n", "117 117 117\n", "118 118 118\n", "119 119 119\n",
    "120 120 120\n", "121 121 121\n", "122 122 122\n", "123 123 123\n",
    "124 124 124\n", "125 125 125\n", "126 126 126\n", "127 127 127\n",
    "128 128 128\n"};

const char colour_str[][13] = {
    "015 006 033\n", // Dark Purple
    "042 007 054\n", // Purple
    "068 017 053\n", // Reddish Purple
    "093 027 042\n", // Red-Orange
    "115 044 028\n", // Bright Orange
    "125 067 019\n", // Orange-Yellow
    "126 095 024\n", // Bright Yellow
    "124 121 087\n", // Pale Yellow
    "128 128 128\n", // White (or light)
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
  // After looking at perf, this method was 6% more effective
  // than just setting "complex float z = x_coord + y_coord * I;"
  complex float complex_z = y_coord * I;
  complex float z = x_coord + complex_z;
  complex float new_z;

  int iter_counter = 128;
  int idx_closest = 0;
  for (int ix = 0; ix < 128; ix++) {

    new_z = calculate_next_iteration(z);
    float new_abs_z =
        crealf(new_z) * crealf(new_z) + cimagf(new_z) * cimagf(new_z);

    char close_to_root = '0';
    
    for (int kx = 0; kx < power; kx++) {
      complex float convergence_of_z = new_z - exact_roots[kx];
      float real_diff = crealf(convergence_of_z);
      float imag_diff = cimagf(convergence_of_z);
      float dist_squared = (real_diff * real_diff) +
                     (imag_diff * imag_diff);
      if (dist_squared < 1e-6) {
        close_to_root = '1';
        idx_closest = kx;
        break;
      }
    }
    if (close_to_root == '1') {
      iter_counter = ix;
      break;
    }

    // Checks if too close or too far away
    else if (new_abs_z < 1e-6) {
      iter_counter = ix;
      break;
    } else if ((fabsf(crealf(new_z)) > 1e10) || (fabsf(cimagf(new_z)) > 1e10)) {
      iter_counter = ix;
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
