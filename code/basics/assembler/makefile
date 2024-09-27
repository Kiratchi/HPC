BINS = main if for int float call

.PHONY : all
all : main.s float.s $(BINS)


main.s : main.c
	gcc -S -fverbose-asm $<

main : main.c
	gcc -o $@ $< -O0 -g

if : if.c
	gcc -o $@ $< -O0 -g

for : for.c
	gcc -o $@ $< -O0 -g

int : int.c
	gcc -o $@ $< -O0 -g

float : float.c
	gcc -o $@ $< -O0 -g

float.s : float.c
	gcc -S -fverbose-asm $<

call : call.c
	gcc -o $@ $< -O0 -g

.PHONY : clean
clean :
	rm -f main.s float.s $(BINS)
