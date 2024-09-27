# NOTE: A typical pitfall for beginners is that the
# callgraph is not provided for too simple examples,
# meaning that functions that are called return to
# quickly.

# This generate an output file gmon.out, since we
# passed the flag -pg to the compiler.
./gprof_example

gprof -b gprof_example > gprof_example.gprof
# Flat profile:
# 
# Each sample counts as 0.01 seconds.
#   %   cumulative   self              self     total
#  time   seconds   seconds    calls  ms/call  ms/call  name
#  42.92      0.52     0.52        2   261.82   493.43  addmul
#  37.97      0.99     0.46     2000     0.23     0.23  addmul_subcall
#  19.81      1.23     0.24        1   241.68   241.68  addadd
#   0.00      1.23     0.00   300000     0.00     0.00  rand_double
#   0.00      1.23     0.00        1     0.00     0.00  addadd_subcall
# 
# 
#                         Call graph
# 
# 
# granularity: each sample hit covers 2 byte(s) for 0.81% of 1.23 seconds
# 
# index % time    self  children    called     name
#                                                  <spontaneous>
# [1]    100.0    0.00    1.23                 main [1]
#                 0.52    0.46       2/2           addmul [2]
#                 0.24    0.00       1/1           addadd [4]
#                 0.00    0.00  300000/300000      rand_double [5]
# -----------------------------------------------
#                 0.52    0.46       2/2           main [1]
# [2]     80.3    0.52    0.46       2         addmul [2]
#                 0.46    0.00    2000/2000        addmul_subcall [3]
# -----------------------------------------------
#                 0.46    0.00    2000/2000        addmul [2]
# [3]     37.7    0.46    0.00    2000         addmul_subcall [3]
# -----------------------------------------------
#                 0.24    0.00       1/1           main [1]
# [4]     19.7    0.24    0.00       1         addadd [4]
#                 0.00    0.00       1/1           addadd_subcall [6]
# -----------------------------------------------
#                 0.00    0.00  300000/300000      main [1]
# [5]      0.0    0.00    0.00  300000         rand_double [5]
# -----------------------------------------------
#                 0.00    0.00       1/1           addadd [4]
# [6]      0.0    0.00    0.00       1         addadd_subcall [6]
# -----------------------------------------------
# 
# 
# Index by function name
# 
#    [4] addadd                  [2] addmul                  [5] rand_double
#    [6] addadd_subcall          [3] addmul_subcall

# Use
# gprof gprof_example > gprof_example.gprof
# to inclde general explanations about each column.
