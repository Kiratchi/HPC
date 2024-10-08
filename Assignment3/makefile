CC = gcc

CFLAGS = -O3 -march=native					# Normal
# CFLAGS = -O3 -march=native 				# Without omp
# CFLAGS = -Og -g -march=native 			# For perf
# CFLAGS = -Og -g -march=native -fopenmp	# For valgrind
LDFLAGS = -std=c11 -lm

.PHONY: all clean
all: newton

newton: newton.c
	$(CC) $(CFLAGS) -o newton newton.c $(LDFLAGS)

clean:
	rm -rf newton
