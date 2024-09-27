CFLAGS = -pg
BINS = perf_example gprof_example massif_example

.PHONY : all
all : $(BINS)

perf_example : perf_example.c
	gcc -o $@ $< -O2 -g

# NOTE: We pass the flag -pg to generate the profiling information at runtime.
gprof_example :gprof_example.c gprof_example_auxiliary_00.c gprof_example_auxiliary_01.c
	gcc -o $@ $^ -O2 -g -pg

massif_example : massif_example.c
	gcc -o $@ $< -O2 -g

.PHONY : clean
clean :
	rm -f $(BINS) \
	  perf.data perf.data.old  gmon.out \
	  cachegrind.out.* callgrind.out.* massif.out.* \
	  perf_example.c.annotated gprof_example.c.annotated massif_example.c.annotated
