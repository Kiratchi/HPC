# Se tilll att ta bort alla #pragma omp
# Och att fixa i makefile så vi har -g och -Og
make
perf record -e cpu-cycles ./distances
perf report