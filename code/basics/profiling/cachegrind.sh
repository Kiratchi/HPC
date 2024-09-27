valgrind --tool=cachegrind ./perf_example

# ==78439== Cachegrind, a cache and branch-prediction profiler
# ==78439== Copyright (C) 2002-2017, and GNU GPL'd, by Nicholas Nethercote et al.
# ==78439== Using Valgrind-3.16.0 and LibVEX; rerun with -h for copyright info
# ==78439== Command: ./perf_example
# ==78439==
# --78439-- warning: L3 cache found, using its data for the LL simulation.
# benchmark time for one iteration: 260.760950mus
# ==78439==
# ==78439== I   refs:      2,802,266,767
# ==78439== I1  misses:            1,193
# ==78439== LLi misses:            1,170
# ==78439== I1  miss rate:          0.00%
# ==78439== LLi miss rate:          0.00%
# ==78439==
# ==78439== D   refs:      1,600,926,572  (1,200,632,140 rd   + 400,294,432 wr)
# ==78439== D1  misses:      200,088,421  (  150,043,967 rd   +  50,044,454 wr)
# ==78439== LLd misses:            7,695  (        2,104 rd   +       5,591 wr)
# ==78439== D1  miss rate:          12.5% (         12.5%     +        12.5%  )
# ==78439== LLd miss rate:           0.0% (          0.0%     +         0.0%  )
# ==78439==
# ==78439== LL refs:         200,089,614  (  150,045,160 rd   +  50,044,454 wr)
# ==78439== LL misses:             8,865  (        3,274 rd   +       5,591 wr)
# ==78439== LL miss rate:            0.0% (          0.0%     +         0.0%  )

# Annotate cache grind results.
# The suffix for the cachegrind output file is the process id that can be found
# to the left of the cachegrind terminal output.
cg_annotate cachegrind.out.78439 perf_example.c > perf_example.c.annotated
