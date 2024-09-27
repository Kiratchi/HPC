# for loops in the shell work in a similar way as in other programming
# languages; variables are accessed by prefixiing them with $
for i in {0..9}; do
	echo -n "$i "
done
echo

# if the separation of variable names and text that follows is not clear you can use ${ }
for var in {0..9}; do
	echo -n "num${var} "
done
echo

for f in *.c9; do
	echo "found c-file: "$f
done
echo
# There is if-statements, break, and continue, which you can look up if you need them at
# https://www.gnu.org/software/bash/manual/
