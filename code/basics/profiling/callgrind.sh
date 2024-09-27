valgrind --tool=callgrind ./gprof_example
# ==84700== Callgrind, a call-graph generating cache profiler
# ==84700== Copyright (C) 2002-2017, and GNU GPL'd, by Josef Weidendorfer et al.
# ==84700== Using Valgrind-3.16.0 and LibVEX; rerun with -h for copyright info
# ==84700== Command: ./gprof_example
# ==84700==
# ==84700== For interactive control, run 'callgrind_control -h'.
# ==84700==
# ==84700== Events    : Ir
# ==84700== Collected : 3520577177
# ==84700==
# ==84700== I   refs:      3,520,577,177

# The suffix for the cachegrind output file is the process id that can be found
# to the left of the cachegrind terminal output.
callgrind_annotate callgrind.out.84700 > gprof_example.annotated
# --------------------------------------------------------------------------------
# Profile data file 'callgrind.out.84700' (creator: callgrind-3.16.0)
# --------------------------------------------------------------------------------
# I1 cache:
# D1 cache:
# LL cache:
# Timerange: Basic block 0 - 504782657
# Trigger: Program termination
# Profiled target:  ./gprof_example (PID 84700, part 1)
# Events recorded:  Ir
# Events shown:     Ir
# Event sort order: Ir
# Thresholds:       99
# Include dirs:
# User annotated:
# Auto-annotation:  on
# 
# --------------------------------------------------------------------------------
# Ir
# --------------------------------------------------------------------------------
# 3,520,555,243 (100.0%)  PROGRAM TOTALS
# 
# --------------------------------------------------------------------------------
# Ir                      file:function
# --------------------------------------------------------------------------------
# 1,400,026,050 (39.77%)  gprof_example_auxiliary_00.c:addmul [gprof_example]
# 1,400,022,000 (39.77%)  gprof_example_auxiliary_01.c:addmul_subcall [gprof_example]
#   700,006,009 (19.88%)  gprof_example_auxiliary_00.c:addadd [gprof_example]
# 
# --------------------------------------------------------------------------------
# -- Auto-annotated source: gprof_example_auxiliary_00.c
# --------------------------------------------------------------------------------
# Ir
# 
# -- line 23 ----------------------------------------
#           .           void
#           .           addmul(
#           .               double *restrict r,
#           .               const double *restrict a,
#           .               const double *restrict b,
#           .               const double *restrict c,
#           .               const int sz
#           .               )
#          32 ( 0.00%)  {
#          62 ( 0.00%)  => ???:mcount (2x)
#       6,000 ( 0.00%)    for ( int bx = 0; bx < bench_iter; ++bx ) {
# 600,008,002 (17.04%)      for ( int ix = 0; ix < sz; ++ix )
# 800,000,000 (22.72%)        r[ix] = a[ix] * b[ix] + c[ix];
#      12,000 ( 0.00%)      addmul_subcall(r, a, b, c, sz);
# 1,400,116,004 (39.77%)  => gprof_example_auxiliary_01.c:addmul_subcall (2,000x)
#           .             }
#          16 ( 0.00%)  }
#           .
#           .           void
#           .           addadd(
#           .               double *restrict r,
#           .               const double *restrict a,
#           .               const double *restrict b,
#           .               const double *restrict c,
#           .               const int sz
#           .               )
#           4 ( 0.00%)  {
#          31 ( 0.00%)  => ???:mcount (1x)
#       2,000 ( 0.00%)    for ( int bx = 0; bx < bench_iter; ++bx )
# 300,004,003 ( 8.52%)      for ( int ix = 0; ix < sz; ++ix )
# 400,000,000 (11.36%)        r[ix] = a[ix] + b[ix] + c[ix];
#           1 ( 0.00%)    addadd_subcall(r, a, b, c, sz);
#     700,042 ( 0.02%)  => gprof_example_auxiliary_01.c:addadd_subcall (1x)
#           1 ( 0.00%)  }
# 
# --------------------------------------------------------------------------------
# -- Auto-annotated source: gprof_example_auxiliary_01.c
# --------------------------------------------------------------------------------
# Ir
# 
#           .           void
#           .           addmul_subcall(
#           .               double *restrict r,
#           .               const double *restrict a,
#           .               const double *restrict b,
#           .               const double *restrict c,
#           .               const int sz
#           .               )
#       8,000 ( 0.00%)  {
#      94,004 ( 0.00%)  => ???:mcount (2,000x)
# 600,010,000 (17.04%)    for ( int ix = 0; ix < sz; ++ix )
# 800,000,000 (22.72%)      r[ix] = a[ix] * b[ix] + c[ix];
#       4,000 ( 0.00%)  }
#           .
#           .           void
#           .           addadd_subcall(
#           .               double *restrict r,
#           .               const double *restrict a,
#           .               const double *restrict b,
#           .               const double *restrict c,
#           .               const int sz
#           .               )
#           4 ( 0.00%)  {
#          31 ( 0.00%)  => ???:mcount (1x)
#     300,005 ( 0.01%)    for ( int ix = 0; ix < sz; ++ix )
#     400,000 ( 0.01%)      r[ix] = a[ix] + b[ix] + c[ix];
#           2 ( 0.00%)  }
# 
# --------------------------------------------------------------------------------
# Ir
# --------------------------------------------------------------------------------
# 3,500,754,070 (99.44%)  events annotated
