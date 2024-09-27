BINS = thread_debug
CFLAGS = -O2 -g -lpthread

.PHONY : all
all : $(BINS) 

thread_debug : thread_debug.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)
