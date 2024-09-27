BINS = pingpong pingpong_thrd pingpong_omp sendrecv deadlock deadlock_circ deadlock_tags deadlock_sendrecv deadlock_isend broadcast reduce scatterv_reduce neighbor communicators communicators_split
LIBRARIES = -L/usr/lib64/openmpi/lib -lm -lmpi -Wl,-rpath,/usr/lib64/openmpi/lib -Wl,--enable-new-dtags
CFLAGS  = -O2 -I. -I/usr/include/openmpi-x86_64 -pthread $(LIBRARIES)

.PHONY : all
all : $(BINS) 

pingpong : pingpong.c
	gcc $(CFLAGS) -o $@ $<

pingpong_omp : pingpong_omp.c
	gcc $(CFLAGS) -fopenmp -o $@ $<

pingpong_thrd : pingpong_thrd.c
	gcc $(CFLAGS) -o $@ $<

sendrecv : sendrecv.c
	gcc $(CFLAGS) -o $@ $<

deadlock : deadlock.c
	gcc $(CFLAGS) -o $@ $<

deadlock_circ : deadlock_circ.c
	gcc $(CFLAGS) -o $@ $<

deadlock_sendrecv : deadlock_sendrecv.c
	gcc $(CFLAGS) -o $@ $<

deadlock_tags : deadlock_tags.c
	gcc $(CFLAGS) -o $@ $<

deadlock_isend : deadlock_isend.c
	gcc $(CFLAGS) -o $@ $<

broadcast : broadcast.c
	gcc $(CFLAGS) -o $@ $<

reduce : reduce.c
	gcc $(CFLAGS) -o $@ $<

scatterv_reduce : scatterv_reduce.c
	gcc $(CFLAGS) -o $@ $<

neighbor : neighbor.c
	gcc $(CFLAGS) -o $@ $<

communicators : communicators.c
	gcc $(CFLAGS) -o $@ $<

communicators_split : communicators_split.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)
