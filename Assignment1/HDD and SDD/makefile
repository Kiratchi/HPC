.PHONY : all
all : HDD_and_SDD_O0 HDD_and_SDD_O2 HDD_and_SDD_O2_native

HDD_and_SDD_O0 : HDD_and_SDD.c 
	gcc -O0 -o $@ $<

HDD_and_SDD_O2 : HDD_and_SDD.c 
	gcc -O2 -o $@ $<

HDD_and_SDD_O2_native : HDD_and_SDD.c 
	gcc -O2 -march=native -o $@ $<
