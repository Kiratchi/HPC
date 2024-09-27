BINS = sequential horz_openmp vert_openmp horz_c11 vert_c11
CFLAGS = -O2 -march=native

.PHONY : all
all : $(BINS)

sequential : sequential.c
	gcc -o $@ $< $(CFLAGS) -fopenmp

horz_openmp : horz_openmp.c
	gcc -o $@ $< $(CFLAGS) -fopenmp

vert_openmp : vert_openmp.c
	gcc -o $@ $< $(CFLAGS) -fopenmp

horz_c11 : horz_c11.c
	gcc -o $@ $< $(CFLAGS) -lpthread

vert_c11 : vert_c11.c
	gcc -o $@ $< $(CFLAGS) -lpthread

.PHONY : clean
clean :
	rm -rf $(BINS)
