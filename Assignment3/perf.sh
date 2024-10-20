make
perf record -e cpu-cycles ./newton -t1 -l1000 2
perf report