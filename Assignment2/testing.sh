#rm cell_distances.tar.gz
rm -r extracted
tar czf cell_distances.tar.gz distances.c  makefile
/home/hpc2024/cell_distances/check_submission.jl /home/hpcuser198/asig2_3/HPC/Assignment2/cell_distances.tar.gz