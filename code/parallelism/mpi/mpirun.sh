# This command is specific to Fedora/RHEL. Other systems might require
# different commands to "load" MPI (or it is loaded already).
module load mpi


# Compilation and linking of MPI based programs requires flags, which are
# provided by the MPI compiler wrapper mpicc.

mpicc -show:compiler
# gcc -I/usr/include/openmpi-x86_64 -pthread -Wl,-rpath -Wl,/usr/lib64/openmpi/lib -Wl,--enable-new-dtags -L/usr/lib64/openmpi/lib -lmpi
mpicc -show:linker
# gcc -I/usr/include/openmpi-x86_64 -pthread -Wl,-rpath -Wl,/usr/lib64/openmpi/lib -Wl,--enable-new-dtags -L/usr/lib64/openmpi/lib -lmpi


mpirun -n 2 pingpong

# Number of processes: 2
# MPI message sent from 0: 1
# MPI message received at 1: 1
# MPI message sent from 1: 2
# MPI message received at 0: 2

mpirun -n 10 ./pingpong
# Number of processes: 10
# MPI message sent from 0: 1
# MPI message received at 0: 2
# MPI message received at 1: 1
# MPI message sent from 1: 2

mpirun -n 1 ./pingpong
# [gantenbein:1989722] *** An error occurred in MPI_Send
# [gantenbein:1989722] *** reported by process [1723269121,0]
# [gantenbein:1989722] *** on communicator MPI_COMM_WORLD
# [gantenbein:1989722] *** MPI_ERR_RANK: invalid rank
# [gantenbein:1989722] *** MPI_ERRORS_ARE_FATAL (processes in this communicator will now abort,
# [gantenbein:1989722] ***    and potentially your MPI job)
# Number of processes: 1

mpirun -n 2 ./sendrecv
# Number of processes: 2
# MPI message sent from 0: 1
# MPI message received at 0: 2
# MPI message sent from 1: 2
# MPI message received at 1: 1


mpirun -n 2 ./pingpong_omp
# Number of processes: 2
# MPI message sent from 0: 1
# MPI message received at 1: 1
# MPI message received at 0: 9
# MPI message sent from 1: 9


mpirun -n 2 ./pingpong_thrd 
# Number of processes: 2
# MPI message sent from 0: 1
# MPI message received at 1: 1
# MPI message received at 0: 9
# MPI message sent from 1: 9


# these hang indefinitely on the test system
mpirun -n 2 ./deadlock
mpirun -n 3 ./deadlock_circ
mpirun -n 2 ./deadlock_tags

mpirun -n 2 ./deadlock_sendrecv.c
# Number of processes: 2
# MPI message sent from 1: 0
# MPI message received at 1: 0
# MPI message sent from 0: 0
# MPI message received at 0: 0

mpirun -n 2 ./deadlock_isend.c
# Number of processes: 2
# MPI message sent from 1: 0
# MPI message sent from 0: 0
# MPI message received at 1: 0
# MPI message received at 0: 0


mpirun -n 3 ./broadcast.c
# MPI received at 0: 1
# MPI received at 2: 1


mpirun -n 3 ./reduce
# MPI received at 1: 3 6 9


mpirun -n 7 ./scatterv_reduce
# len pos 1428572 0
# len pos 1428572 1428572
# len pos 1428572 2857144
# len pos 1428572 4285716
# len pos 1428572 5714288
# len pos 1428572 7142860
# len pos 1428568 8571432
# Global maximum 9999999.000000 at location 9999999


mpirun -n 3 ./neighbor
# no topology defined at 1
# no topology defined at 0
# no topology defined at 2

# When skipping the check for MPI topologies instead
mpirun -n 2 ./neighbor
# [gantenbein:2004818] *** An error occurred in MPI_Neighbor_allgather
# [gantenbein:2004818] *** reported by process [2913796097,1]
# [gantenbein:2004818] *** on communicator MPI_COMM_WORLD
# [gantenbein:2004818] *** MPI_ERR_TOPOLOGY: invalid communicator topology
# [gantenbein:2004818] *** MPI_ERRORS_ARE_FATAL (processes in this communicator will now abort,
# [gantenbein:2004818] ***    and potentially your MPI job)
# [gantenbein:2004817] *** An error occurred in MPI_Neighbor_allgather
# [gantenbein:2004817] *** reported by process [2913796097,0]
# [gantenbein:2004817] *** on communicator MPI_COMM_WORLD
# [gantenbein:2004817] *** MPI_ERR_TOPOLOGY: invalid communicator topology
# [gantenbein:2004817] *** MPI_ERRORS_ARE_FATAL (processes in this communicator will now abort,
# [gantenbein:2004817] ***    and potentially your MPI job)


mpirun -n 7 ./communicators
# MPI received at 0: 1
# MPI received at 6: 1


mpirun -n 7 ./communicators_split
# mpi_rank, sub_group_rank, sub_group_size: 0 0 4
# mpi_rank, sub_group_rank, sub_group_size: 1 1 4
# mpi_rank, sub_group_rank, sub_group_size: 2 2 4
# mpi_rank, sub_group_rank, sub_group_size: 3 3 4
# mpi_rank, sub_group_rank, sub_group_size: 4 1 2
# mpi_rank, sub_group_rank, sub_group_size: 5 0 1
# mpi_rank, sub_group_rank, sub_group_size: 6 0 2
