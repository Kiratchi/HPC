.PHONY : all
all : indirect_addressing_O0 indirect_addressing_O2 indirect_addressing_O2_native

indirect_addressing_O0 : indirect_addressing.c 
	gcc -O0 -o $@ $<

indirect_addressing_O2 : indirect_addressing.c 
	gcc -O2 -o $@ $<

indirect_addressing_O2_native : indirect_addressing.c 
	gcc -O2 -march=native -o $@ $<

# clean : 
# 	rm -rf indirect_addressing_O0
