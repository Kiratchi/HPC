# grep is a program to find strings in on or several files.

# list all lines that contain of the string "char" in the file atomics.h
grep char /usr/include/julia/atomics.h

# list all lines that contain of the string "char" or alternative capitalization
grep -i julia /usr/include/julia/atomics.h

# list all lines that contain of the string "char" in all files ending with .h
# in the folder julia (many hits)
grep char /usr/include/julia/*.h

# list all lines that contain of the string "char" in in the files atomics.h and dtypes.h
grep char /usr/include/julia/atomics.h  /usr/include/julia/dtypes.h

# list all files among those ending with .h that contain of the string "char"
grep -l char /usr/include/julia/*.h

# to print more than the line, but also previous or succeeding lines use the flags
# -A   after, lines after the hit
# -B   before, lines before the hit
# -C   center, context, lines before and after, centering the hit
grep -C5 char /usr/include/julia/dtypes.h

# to count the number of occurrences use the flag -c
grep -c char /usr/include/julia/*.h

# to include the line number of each hit use the flag -n
grep -n char /usr/include/julia/atomics.h
