#!/bin/bash

# Basic sample to read a file line by line
# and process each line

FILENAME=findByDate

while read f
do
   echo "X: ${f}"
done < ${FILENAME}
