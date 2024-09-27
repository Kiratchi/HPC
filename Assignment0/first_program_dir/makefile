.PHONY : all
all : first_program second_executable

first_program : first_program.c
	echo "build first_program"
	gcc -o first_program first_program.c

second_executable : first_program.c potential_header.h
	gcc -o second_executable first_program.c