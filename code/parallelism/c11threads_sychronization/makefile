BINS = none atomic mutex reduction spin condvar semaphore
CFLAGS = -O2 -g

.PHONY : all
all : $(BINS) 

none : none.c
	gcc $(CFLAGS) -o $@ $< -lpthread

atomic : atomic.c
	gcc $(CFLAGS) -o $@ $< -lpthread

mutex : mutex.c
	gcc $(CFLAGS) -o $@ $< -lpthread

reduction : reduction.c
	gcc $(CFLAGS) -o $@ $< -lpthread

spin : spin.c
	gcc $(CFLAGS) -o $@ $< -lpthread

condvar : condvar.c
	gcc $(CFLAGS) -o $@ $< -lpthread

semaphore : semaphore.c
	gcc $(CFLAGS) -o $@ $< -lpthread

.PHONY : clean
clean :
	rm -rf $(BINS)
