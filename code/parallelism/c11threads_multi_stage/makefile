BINS = multi_stage
CFLAGS = -O2 -g

.PHONY : all
all : $(BINS) 

multi_stage : multi_stage.c
	gcc $(CFLAGS) -o $@ $< -lpthread -lm

.PHONY : clean
clean :
	rm -rf $(BINS)
