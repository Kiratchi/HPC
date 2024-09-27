# - the GNU make manual starts has its first section at
#   https://www.gnu.org/software/make/manual/make.html#Rule-Introduction
# - comments in makefiles are prefaced by #

# The form of a rule in make is
# TARGET : PREREQUISITS
# 	RECIPE
# The recipe must be indended by TABS as opposed to any other whitespace.


.PHONY : all
all : first_executable second_executable

first_executable : first_c_program.c
	gcc -o first_executable first_c_program.c

second_executable : first_c_program.c potential_header.h
	gcc -o second_executable first_c_program.c
