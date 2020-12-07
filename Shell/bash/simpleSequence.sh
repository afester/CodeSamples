#!/bin/bash

SIMPLE_SEQ="A B,x,y,z D E"

iterate() {
   SEQ=$1
   ITER=$2

   for i in ${SEQ} ; do
      idx=0 
      for x in `echo $i | sed -e "s/,/ /g"` ; do
         $ITER $idx $x
        idx=$((idx + 1))
      done
   done
}

iterator() {
   echo "ITERATE: $*"
}

iterate "${SIMPLE_SEQ}" iterator
