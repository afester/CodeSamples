#!/bin/bash

# Removes a comment header from a set of files.
# The header must start with /* at the beginning of the file,
# and it is removed up to the next */. 
# Additional white spaces after the terminating */ are also removed.

if [ $# -ne 1 ] ; then
    echo "$0 pattern"
    exit 1
fi

PATTERN=$1

for f in `find . -name "${PATTERN}"`; do
   echo $f
   
   # -0777  puts Perl into file slurp mode
   # -i     Enable in-place-editing (string parameter is extension of original file. .bak is used if nothing specified.)
   # -p     Add an implicit line processing loop and print each processed line
   # -e     Executes perl code specified on the command line
   perl -0777 -i -pe 's/\A\/\*.*?\*\/\s*//igs' ${f}
done
