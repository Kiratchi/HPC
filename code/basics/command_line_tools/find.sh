# the command find lets you find all files matching a specific description
# performing specifc operations on them

# find and print all files and directories in the folder julia
find /usr/include/julia

# find and print all directories in the folder include
# -type f mataches files
find -type d /usr/include

# find and print all files in include that start with u end on v and have an
# arbitrary file name extension
find /usr/include -type f -name v\*v.\*
find /usr/include -type f -name "v*v.*"

# to execture a command on the files found combine the flag -exec with the {}
# to indicate the file name and end the command with \;
find /usr/include -name "*.h" -exec grep -li julia {} \;

# there is many more possibilities which are described in the man page of find
man find

# for example to replace "FOUND" by "found" in each c- and h-file one can use
# find . -regex ".*\.[hc]" -exec sed -i -e "s/FOUND/found/g" {} \;
