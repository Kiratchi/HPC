BINS = dotprod matmul dotprod_float4 dotprod_reduction
CFLAGS = -O2 -lOpenCL

.PHONY : all
all : $(BINS) 

dotprod : dotprod.c
	gcc $(CFLAGS) -o $@ $<

matmul : matmul.c
	gcc $(CFLAGS) -o $@ $<

dotprod_float4 : dotprod_float4.c
	gcc $(CFLAGS) -o $@ $<

dotprod_reduction : dotprod_reduction.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)
