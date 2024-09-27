# the command xargs converts text from the "stdin stream" to arguments for a
# command line program. 

printf "00\n07\n" > nmbs
cat nmbs | xargs echo "printing "

# you can determine the (maximal) number of arguments provided to each call by the flag -n
cat nmbs | xargs -n 1 echo "printing "

printf "00\n07\n08\n12\n" > nmbs
cat nmbs | xargs -n 2 echo "printing "

# when combining xargs with for example find null-terminated strings can be used with the flag -0
find . -type f -name \*.c -print0 | xargs -0 -n 1 echo

# to avoid running any command, if no input is provided use the flag -r
find . -type f -name \*.c -print0 | xargs -0 -n 1 -r echo

# xargs can run several program in parallel when invoked with the flag -P
for n in {00..99}; do printf "int f(int a) { return 2*a; }" > $n.c; done
find . -type f -name \*.c -print0 | xargs -0 -P 4 -n 1  gcc -c

# if the argument of the invoked command does not come last, the flag -I provides a solution
find . -type f -name \*.c -print0 | xargs -0 -P 4 -n 1 -I FILE gcc FILE -c

# to print the commands executed to the terminal use the flag -t
find . -type f -name \*.c -print0 | xargs -0 -P 4 -n 1 -t gcc -c

echo {00..99}.{c,o}
rm {00..99}.{c,o}

# xargs provide much more flexibility and the man page describes more flags
