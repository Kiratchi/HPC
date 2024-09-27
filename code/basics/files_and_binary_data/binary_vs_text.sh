# The command cat outputs a text file to the terminal. When applying it
# to binary data, the result is unintelligable.
cat test_file.dat

# To print binary data as a series of two-digit hex numbers one can use xxd.
xxd test_file.dat

cat test_file.txt

# One can also examine text files as binary data, whihc reveals the ASCII
# encoded characters.
xxd test_file.txt
