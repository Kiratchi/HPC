hyperfine ./hyperfine_bench

#  Benchmark #1: ./hyperfine_bench
#    Time (mean ± σ):      26.5 ms ±   1.6 ms    [User: 21.0 ms, System: 5.3 ms]
#    Range (min … max):    24.2 ms …  29.2 ms    108 runs

# The argument warmup is useful if a program needs to heat up the cache or CPU.
hyperfine --warmup 10 ./hyperfine_bench

# Benchmark #1: ./hyperfine_bench
#   Time (mean ± σ):      26.4 ms ±   1.9 ms    [User: 21.4 ms, System: 4.9 ms]
#   Range (min … max):    23.7 ms …  31.1 ms    101 runs


# The argument max-runs is useful if a program takes too long time or consumes
# valuable resources such that it is preferable to have less precise benchmark
# over running the program often.
hyperfine --max-runs 20 ./hyperfine_bench

# Benchmark #1: ./hyperfine_bench
#   Time (mean ± σ):      25.9 ms ±   1.5 ms    [User: 21.4 ms, System: 4.3 ms]
#   Range (min … max):    23.5 ms …  29.1 ms    20 runs
