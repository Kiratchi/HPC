CC = gcc

CFLAGS = -O3 -lpthread -march=native		# Normal
# CFLAGS = -Og -g -march=native 			# For perf
# CFLAGS = -Og -g -march=native -fopenmp	# For valgrind
LDFLAGS = -lm #-std=c11

.PHONY: all clean
all: newton

newton: newton.c
	$(CC) $(CFLAGS) -o newton newton.c $(LDFLAGS)

convert: 
	for file in *.ppm; do \
		magick $$file $${file%.ppm}.png; \
	done

clean:
	rm -rf newton
	rm -f newton_attractors_x*.ppm
	rm -f newton_convergence_x*.ppm
	rm -f newton_attractors_x*.png
	rm -f newton_convergence_x*.png

submit: clean
	rm -r extracted
	tar czf submit.tar.gz newton.c makefile
	/home/hpc2024/newton_iteration/check_submission.jl submit.tar.gz
