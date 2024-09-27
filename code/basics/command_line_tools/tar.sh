# tar is both a command and a format. The latter is an archiving format (tape
# archive), collecting a zero or more files for the purpose of archiving,
# compression, tranmission etc. The command tar creates or extracts tar-files.

# As a side remark: the shell expands several braces in all possible
# combinations with precedence to the right.
echo {00..99}.{c,h}
for f in {00..99}.{c,h}; do echo "test text" > $f; done

# To create an archive use the argument c and provide a list of files that
# shall be included in the archive. The argument preceeds a target file.
tar cf files.tar {00..99}.{c,h}

# The resulting file files.tar is an uncompressed tape archive.
# To create a compressed argive use the following arguments
# z gunzip (fast)
# j bzip2 (repetative data, but often superceeded by xz)
# J xz (large files)

tar czf files.tar.gz {00..99}.{c,h}
tar cjf files.tar.bz2 {00..99}.{c,h}
tar cJf files.tar.xz {00..99}.{c,h}

# To extract (and uncompressed if needed) a tape archive proivde the argument x.
# The archive will be uncompressed depending on its extension.

tar xf files.tar
tar xf files.tar.gz
tar xf files.tar.bz2
tar xf files.tar.xz

# To list the content of an archive without extracting it, use the argument t (tabulate).
tar tf files.tar | grep 00

# The next combination of commands determines which files exist in both the
# archive and the current working directory. This can be helpful to avoid
# overwriting files when unpacking.
(tar tf files.tar; ls -1) | sort | uniq -cd
