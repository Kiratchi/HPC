# to print text to the terminal use the command echo
echo "printed text"

# use the flag -n to suppress the newline at the end
echo -n "printed text"

# the same purpose is fullfilled by the command printf, but it has more consistent behavior
printf "00\n01\n"

# to output the content of one or several text files use the command cat
cat /usr/include/julia/julia_version.h

# if the output of a command is too long, you can employ the command less
# together with a shell "pipeline" instruction
cat /usr/include/julia/uv.h | less
grep char /usr/include/julia/uv.h | less
