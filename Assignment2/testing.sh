#rm cell_distances.tar.gz
rm -r extracted
tar czf cell_distances.tar.gz distances.c makefile
# /home/hpc2024/cell_distances/check_submission.jl /home/hpcuser198/asig2_3/HPC/Assignment2/cell_distances.tar.gz             # FANFAR
/home/hpc2024/cell_distances/check_submission.jl /home/hpcuser139/tma/assignment2and3/Assignment2/cell_distances.tar.gz     # Jaffa
# /home/hpc2024/cell_distances/check_submission.jl /home/hpcuser134/lé\ code/Assignment2/cell_distances.tar.gz               # Pumba
