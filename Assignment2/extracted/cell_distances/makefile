
CC = gcc


CFLAGS = -O3 -march=native -fopenmp		# Normal
#CFLAGS = -Og -g -march=native -fopenmp		# For valgrind
# CFLAGS = -O3 -march=native 			# Without omp
# CFLAGS = -Og -g -march=native 		# For perf
LDFLAGS = -lm

.PHONY: all clean
all: distances

distances: distances.c
	$(CC) $(CFLAGS) -o distances distances.c $(LDFLAGS)

clean:
	rm -rf distances
