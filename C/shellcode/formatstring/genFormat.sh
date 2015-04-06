#!/bin/bash

X=""
for f in `seq 1 200`
do
   X="$X %08x"
done
X="AAAAAAAAAAAAAAAAAAA$X"

echo $X
