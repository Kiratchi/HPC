BINS = none_openmp none_c11 atomic_openmp atomic_c11 critical_openmp mutex_c11 reduction_openmp reduction_c11
CFLAGS = -O2

.PHONY : all
all : $(BINS) 

none_openmp : none_openmp.c
	gcc $(CFLAGS) -o $@ $< -fopenmp

none_c11 : none_c11.c
	gcc $(CFLAGS) -o $@ $< -lpthread

atomic_openmp : atomic_openmp.c
	gcc $(CFLAGS) -o $@ $< -fopenmp

atomic_c11 : atomic_c11.c
	gcc $(CFLAGS) -o $@ $< -lpthread

critical_openmp : critical_openmp.c
	gcc $(CFLAGS) -o $@ $< -fopenmp

mutex_c11 : mutex_c11.c
	gcc $(CFLAGS) -o $@ $< -lpthread

reduction_openmp : reduction_openmp.c
	gcc $(CFLAGS) -o $@ $< -fopenmp

reduction_c11 : reduction_c11.c
	gcc $(CFLAGS) -o $@ $< -lpthread

.PHONY : bench
bench : all
	@echo "no synchronization (wrong result)"
	hyperfine ./none_openmp
	hyperfine ./none_c11
	@echo
	@echo "mutex synchronization"
	hyperfine ./critical_openmp
	hyperfine ./mutex_c11
	@echo
	@echo "atomic synchronization"
	hyperfine ./atomic_openmp
	hyperfine ./atomic_c11
	@echo
	@echo "reduction synchronization"
	hyperfine ./reduction_openmp
	hyperfine ./reduction_c11

.PHONY : clean
clean :
	rm -rf $(BINS)

# no synchronization (wrong result)
# hyperfine ./none_openmp
# Benchmark #1: ./none_openmp
#   Time (mean ± σ):      23.9 ms ±   5.2 ms    [User: 38.7 ms, System: 12.7 ms]
#   Range (min … max):    21.5 ms …  46.0 ms    76 runs
# hyperfine ./none_c11
# Benchmark #1: ./none_c11
#   Time (mean ± σ):      22.8 ms ±   0.6 ms    [User: 28.3 ms, System: 12.8 ms]
#   Range (min … max):    22.1 ms …  26.9 ms    124 runs
# 
# 
# mutex synchronization
# hyperfine ./critical_openmp
# Benchmark #1: ./critical_openmp
#   Time (mean ± σ):     414.9 ms ±  52.6 ms    [User: 3.038 s, System: 0.012 s]
#   Range (min … max):   351.2 ms … 481.5 ms    10 runs
# 
# hyperfine ./mutex_c11
# Benchmark #1: ./mutex_c11
#   Time (mean ± σ):     567.0 ms ±  54.9 ms    [User: 1.622 s, System: 2.237 s]
#   Range (min … max):   516.4 ms … 649.6 ms    10 runs
# 
# 
# atomic synchronization
# hyperfine ./atomic_openmp
# Benchmark #1: ./atomic_openmp
#   Time (mean ± σ):     152.2 ms ±  20.6 ms    [User: 1.045 s, System: 0.012 s]
#   Range (min … max):   112.8 ms … 170.0 ms    17 runs
# 
# hyperfine ./atomic_c11
# Benchmark #1: ./atomic_c11
#   Time (mean ± σ):     121.8 ms ±   6.6 ms    [User: 768.3 ms, System: 11.5 ms]
#   Range (min … max):   110.3 ms … 138.5 ms    26 runs
# 
# 
# reduction synchronization
# hyperfine ./reduction_openmp
# Benchmark #1: ./reduction_openmp
#   Time (mean ± σ):      22.6 ms ±   0.9 ms    [User: 31.6 ms, System: 12.5 ms]
#   Range (min … max):    21.6 ms …  26.9 ms    107 runs
# 
# hyperfine ./reduction_c11
# Benchmark #1: ./reduction_c11
#   Time (mean ± σ):      21.7 ms ±   0.6 ms    [User: 25.8 ms, System: 13.0 ms]
#   Range (min … max):    20.7 ms …  24.9 ms    135 runs
