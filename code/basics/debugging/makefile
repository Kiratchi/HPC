.PHONY : all
all : gdb_example gdb_example_noassertion valgrind_example

gdb_example : gdb_example.c
	gcc -o gdb_example gdb_example.c -g -Og

gdb_example_noassertion : gdb_example.c
	gcc -o gdb_example_noassertion gdb_example.c -g -Og -D NDEBUG

valgrind_example : valgrind_example.c
	gcc -o valgrind_example valgrind_example.c -g
