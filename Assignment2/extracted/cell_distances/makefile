
CC = gcc


CFLAGS = -O3 -march=native -fopenmp		# Normal
# CFLAGS = -O3 -march=native 				# Without omp
# CFLAGS = -Og -g -march=native 			# For perf
# CFLAGS = -Og -g -march=native -fopenmp	# For valgrind
LDFLAGS = -lm

.PHONY: all clean
all: distances

distances: distances.c
	$(CC) $(CFLAGS) -o distances distances.c $(LDFLAGS)

clean:
	rm -rf distances
