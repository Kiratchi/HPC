BINS = thread_main_void thread_main_array thread_main_struct
CFLAGS = -O2 -lpthread

.PHONY : all
all : $(BINS) 

thread_main_void : thread_main_void.c
	gcc $(CFLAGS) -o $@ $<

thread_main_array : thread_main_array.c
	gcc $(CFLAGS) -o $@ $<

thread_main_struct : thread_main_struct.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)
