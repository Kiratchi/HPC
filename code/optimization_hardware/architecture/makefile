.PHONY : all
all : memory_bound memory_bound_random cpu_bound

memory_bound : memory_bound.c
	gcc -o $@ $< -O2 -march=native

memory_bound_random : memory_bound_random.c
	gcc -o $@ $< -O2 -march=native

cpu_bound : cpu_bound.c
	gcc -o $@ $< -O2 -march=native -lm

.PHONY : clean
clean :
	rm -f  memory_bound memory_bound_random cpu_bound
